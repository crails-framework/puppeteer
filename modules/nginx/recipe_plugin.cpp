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

bool NginxPlugin::ensure_plugin_enabled(const map<string, string>& variables, Sync::Stream& stream) const
{
  if (variables.find("NGINX_DISABLED") != variables.end())
  {
    if (variables.at("NGINX_DISABLED") == "1")
    {
      stream << "(!) NGINX plugin disabled\n";
      return false;
    }
  }
  return true;
}

void NginxPlugin::apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  stream << '\n' << "## NGINX plugin\n";
  if (ensure_plugin_enabled(variables, stream))
  {
    if (variables.find("NGINX_SERVER_IP") != variables.end())
    {
      nginx_machine.set_ip(variables.at("NGINX_SERVER_IP"));
      if (package == "setup")
        setup(recipe_folder, instance, variables, stream);
      else if (package == "uninstall")
        uninstall(recipe_folder, instance, stream);
      reload_nginx(stream);
      if (letsencrypt.recipe_uses_plugin(recipe_folder))
        letsencrypt.apply(package, recipe_folder, instance, variables, stream);
    }
    else
      stream << "cannot manage nginx configuration: missing NGINX_SERVER_IP environment variable\n";
  }
}

void NginxPlugin::setup(const string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  const std::string config = generate_file_from_template(recipe_folder + "/nginx.conf", variables);

  nginx_machine.open_ssh([&](Ssh::Session& ssh)
  {
    stream << "scp nginx.conf to " << nginx_site_path << '/' << instance.get_name() << '\n';
    ssh.make_scp_session(nginx_site_path, SSH_SCP_WRITE)
      ->push_text(config, instance.get_name());
  });
}

void NginxPlugin::uninstall(const string& recipe_folder, Instance& instance, Sync::Stream& stream) const
{
  nginx_machine.open_ssh([&](Ssh::Session& ssh)
  {
    stream << "rm " << nginx_site_path << '/' << instance.get_name() << '\n';
    ssh.exec("rm '" + nginx_site_path +  '/' + instance.get_name() + '\'', stream);
  });
}
