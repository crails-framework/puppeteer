#ifndef  NGINX_LETSENCRYPT_PLUGIN_HPP
# define NGINX_LETSENCRYPT_PLUGIN_HPP

#include "app/models/recipe.hpp"
#include "with_nginx_machine.hpp"

struct NginxLetsencryptPlugin : public Recipe::Plugin, private WithNginxMachine
{
  bool recipe_uses_plugin(const std::string& recipe_path) const;
  void apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream& stream) const;

private:
  bool check_required_variables_presence(const std::map<std::string, std::string>&, Sync::Stream&) const;

  void setup(const std::string& recipe_folder, Instance&, const std::map<std::string, std::string>&, Sync::Stream&) const;
  void uninstall(const std::string& recipe_folder, Instance&, const std::map<std::string, std::string>&, Sync::Stream&) const;
  void refresh_certificates(Instance&, const std::map<std::string, std::string>&, Sync::Stream&) const;

  const std::string get_certificates_directory(const std::map<std::string, std::string>& variables) const;
  const std::string get_site_letsencrypt_relative_filepath(const Instance& instance) const;
  const std::string get_site_letsencrypt_filepath(const Instance& instance) const;
  const std::string generate_letsencrypt_file(const std::map<std::string, std::string>& variables) const;
  const std::string generate_configuration_file(const std::map<std::string, std::string>& variables) const;
  const std::string get_webroot(const std::map<std::string, std::string>& variables) const;
};

#endif
