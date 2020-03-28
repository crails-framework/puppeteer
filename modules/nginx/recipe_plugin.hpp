#ifndef  NGINX_RECIPE_PLUGIN_HPP
# define NGINX_RECIPE_PLUGIN_HPP

#include "app/models/recipe.hpp"
#include "letsencrypt_plugin.hpp"
#include "with_nginx_machine.hpp"

struct NginxPlugin : public Recipe::Plugin, private WithNginxMachine
{
  bool recipe_uses_plugin(const std::string& recipe_path) const;
  void apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream& stream) const;

private:
  NginxLetsencryptPlugin letsencrypt;

  bool ensure_plugin_enabled(const std::map<std::string, std::string>& variables, Sync::Stream&) const;
  void setup    (const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream& stream) const;
  void uninstall(const std::string& recipe_folder, Instance& instance, Sync::Stream& stream) const;
};

#endif
