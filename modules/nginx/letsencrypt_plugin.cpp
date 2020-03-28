#include "letsencrypt_plugin.hpp"
#include "app/models/instance.hpp"
#include "app/ssh/stream.hpp"
#include "app/ssh/session.hpp"
#include "app/ssh/scp.hpp"
#include <boost/filesystem.hpp>
#include <crails/getenv.hpp>

#define PLUGIN_INCLUDE_PATH std::string("/etc/nginx/puppeteer-letsencrypt")
#define PLUGIN_DIRECTIVE std::string("include /etc/nginx/letsencrypt.conf*;")

using namespace std;

bool NginxLetsencryptPlugin::recipe_uses_plugin(const std::string& recipe_path) const
{
  string nginx_conf_path = recipe_path + "/nginx.conf";
  boost::filesystem::path p(nginx_conf_path.c_str());

  if (boost::filesystem::is_regular_file(p))
  {
    string content;

    boost::filesystem::load_string_file(p, content);
    return content.find(PLUGIN_DIRECTIVE) != string::npos;
  }
  return false;
}

bool NginxLetsencryptPlugin::check_required_variables_presence(const map<string, string>& variables, Sync::Stream& stream) const
{
  bool success = true;

  if (variables.find("NGINX_LETSENCRYPT_DISABLED") != variables.end())
  {
    if (variables.at("NGINX_LETSENCRYPT_DISABLED") == "1")
    {
      stream << "(!) Let's Encrypt plugin is disabled\n";
      return false;
    }
  }
  if (variables.find("NGINX_SERVER_IP") == variables.end())
  {
    stream << "missing NGINX_SERVER_IP environment variable\n";
    success = false;
  }
  if (variables.find("NGINX_LETSENCRYPT_WEBROOT") == variables.end())
  {
    stream << "missing NGINX_LETSENCRYPT_WEBROOT environment variable\n";
    success = false;
  }
  if (variables.find("HOSTNAME") == variables.end())
  {
    stream << "missing HOSTNAME environment variable\n";
    success = false;
  }
  return success;
}

void NginxLetsencryptPlugin::apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  stream << "## NGINX Let's Encrypt plugin\n";
  if (check_required_variables_presence(variables, stream))
  {
    nginx_machine.set_ip(variables.at("NGINX_SERVER_IP"));
    if (package == "setup" || package == "uninstall")
    {
      if (package == "setup")
        setup(recipe_folder, instance, variables, stream);
      else if (package == "uninstall")
        uninstall(recipe_folder, instance, variables, stream);
      reload_nginx(stream);
    }
    if (package == "refresh-ssl-certificates" || package == "setup")
    {
      refresh_certificates(instance, variables, stream);
      reload_nginx(stream);
    }
  }
}

const string NginxLetsencryptPlugin::get_certificates_directory(const map<string, string>& variables) const
{
  return "/etc/letsencrypt/live/" + variables.at("HOSTNAME");
}

const string NginxLetsencryptPlugin::get_site_letsencrypt_relative_filepath(const Instance& instance) const
{
  return instance.get_name() + ".conf";
}

const string NginxLetsencryptPlugin::get_site_letsencrypt_filepath(const Instance& instance) const
{
  return PLUGIN_INCLUDE_PATH + "/" + get_site_letsencrypt_relative_filepath(instance);
}

const string NginxLetsencryptPlugin::generate_letsencrypt_file(const map<string, string>& variables) const
{
  const auto directory = get_certificates_directory(variables);
  stringstream stream;

  stream << "ssl_certificate     " << directory << "/fullchain.pem;" << std::endl;
  stream << "ssl_certificate_key " << directory << "/privkey.pem;" << std::endl;
  return stream.str();
}

const string NginxLetsencryptPlugin::generate_configuration_file(const map<string, string>& variables) const
{
  stringstream stream;

  stream << "location /.well-known {" << std::endl;
  stream << "  root " << get_webroot(variables) << ';' << std::endl;
  stream << '}' << std::endl;
  return stream.str();
}

const string NginxLetsencryptPlugin::get_webroot(const map<string, string>& variables) const
{
  return variables.at("NGINX_LETSENCRYPT_WEBROOT");
}

void NginxLetsencryptPlugin::setup(const string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  const string nginx_dir = "/etc/nginx";
  const string nginx_server_ip = variables.at("NGINX_SERVER_IP");

  nginx_machine.open_ssh([&](Ssh::Session& ssh)
  {
    stream << "scp letsencrypt.conf to " << nginx_dir << '\n';
    ssh.make_scp_session(nginx_dir, SSH_SCP_WRITE)
      ->push_text(generate_configuration_file(variables), "letsencrypt.conf");
  });
}

void NginxLetsencryptPlugin::uninstall(const string& recipe_folder, Instance& instance, const map<string, string>& variables, Sync::Stream& stream) const
{
  const string nginx_server_ip = variables.at("NGINX_SERVER_IP");
  const string filepath = get_site_letsencrypt_filepath(instance);
  const string rm_conf_command = "rm '" + filepath + '\'';
  const string rm_cert_command = "rm -R '" + get_certificates_directory(variables) + '\'';

  nginx_machine.open_ssh([&](Ssh::Session& ssh)
  {
    stream << rm_conf_command << '\n';
    ssh.exec(rm_conf_command, stream);
    stream << rm_cert_command << '\n';
    ssh.exec(rm_cert_command, stream);
  });
}

void NginxLetsencryptPlugin::refresh_certificates(Instance& instance, const map<string, string>& variables, Sync::Stream& stream) const
{
  const string site_letsencrypt_path = get_site_letsencrypt_filepath(instance);
  stringstream fetch_cert_command;
  stringstream check_cert_command; 

  fetch_cert_command << "certbot certonly --webroot -n ";
  fetch_cert_command << "-w '" << get_webroot(variables) << "' ";
  fetch_cert_command << "-d " << variables.at("HOSTNAME");
  check_cert_command << "ls '" << get_certificates_directory(variables) << '\'';
  nginx_machine.open_ssh([&](Ssh::Session& ssh)
  {
    stream << fetch_cert_command.str() << '\n';
    ssh.exec(fetch_cert_command.str(), stream);
    stream << check_cert_command.str() << '\n';
    if (ssh.exec(check_cert_command.str(), stream) == 0)
    {
      const string site_letsencrypt_conf = generate_letsencrypt_file(variables);

      ssh.exec("mkdir -p '" + PLUGIN_INCLUDE_PATH + '\'', stream);
      ssh.make_scp_session(PLUGIN_INCLUDE_PATH, SSH_SCP_WRITE)
        ->push_text(site_letsencrypt_conf, get_site_letsencrypt_relative_filepath(instance));
    }
    else
    {
      stream << "(!) Failed to setup ssl certificate\n";
      ssh.exec("rm '" + site_letsencrypt_path + '\'', stream);
    }
  });
}
