#ifndef  NGINX_RECIPE_PLUGIN_HPP
# define NGINX_RECIPE_PLUGIN_HPP

#include "app/models/recipe.hpp"

struct NginxPlugin : public Recipe::Plugin
{
  bool recipe_uses_plugin(const std::string& recipe_path) const;
  void apply(const std::string& package, const std::string& recipe_folder, Instance& instance, Sync::Stream& stream) const;

private:
  void setup    (const std::string& recipe_folder, Instance& instance, Sync::Stream& stream) const;
  void uninstall(const std::string& recipe_folder, Instance& instance, Sync::Stream& stream) const;
};

#endif
