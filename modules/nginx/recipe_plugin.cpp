#include "recipe_plugin.hpp"
#include "app/models/instance.hpp"
#include "app/ssh/stream.hpp"
#include "app/ssh/session.hpp"
#include "app/ssh/scp.hpp"
#include <boost/filesystem.hpp>
#include <crails/getenv.hpp>

using namespace std;

std::string generate_file_from_template(const std::string& template_path, std::map<std::string,std::string> variables);

const std::string nginx_site_path = "/etc/nginx/sites-enabled";

bool NginxPlugin::recipe_uses_plugin(const std::string& recipe_path) const
{
  std::string nginx_conf_path = recipe_path + "/nginx.conf";
  boost::filesystem::path p(nginx_conf_path.c_str());

  return boost::filesystem::is_regular_file(p);
}

void NginxPlugin::apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  stream << '\n' << "## NGINX plugin\n";
  if (package == "setup")
    setup(recipe_folder, instance, variables, stream);
  else if (package == "uninstall")
    uninstall(recipe_folder, instance, stream);
}

void NginxPlugin::setup(const string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  std::string nginx_server_ip = Crails::getenv("NGINX_SERVER_IP");

  if (nginx_server_ip.length())
  {
    std::string config = generate_file_from_template(recipe_folder + "/nginx.conf", variables);
    Ssh::Session ssh;

    ssh.should_accept_unknown_hosts(true);
    ssh.connect(Recipe::remote_user, nginx_server_ip);
    ssh.authentify_with_pubkey();
    stream << "scp nginx.conf to " << nginx_site_path << '/' << instance.get_name() << '\n';
    ssh.make_scp_session(nginx_site_path, SSH_SCP_WRITE)
      ->push_text(config, instance.get_name());
  }
  else
    stream << "cannot deploy nginx configuration: missing NGINX_SERVER_IP environment variable\n";
}

void NginxPlugin::uninstall(const string& recipe_folder, Instance& instance, Sync::Stream& stream) const
{
  std::string nginx_server_ip = Crails::getenv("NGINX_SERVER_IP");

  if (nginx_server_ip.length())
  {
    Ssh::Session ssh;

    ssh.should_accept_unknown_hosts(true);
    ssh.connect(Recipe::remote_user, nginx_server_ip);
    ssh.authentify_with_pubkey();
    stream << "rm " << nginx_site_path << '/' << instance.get_name() << '\n';
    ssh.exec("rm '" + nginx_site_path +  '/' + instance.get_name() + '\'', stream);
  }
  else
    stream << "cannot remove nginx configuration: missing NGINX_SERVER_IP environment variable\n";
}
