#ifndef  GANDI_RECIPE_PLUGIN_HPP
# define GANDI_RECIPE_PLUGIN_HPP

# include "app/models/recipe.hpp"

struct GandiPlugin : public Recipe::Plugin
{
  static const int         domain_zone_id;
  static const std::string domain_name;

  bool recipe_uses_plugin(const std::string& recipe_path) const { return true; }
  void apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream& stream) const;

private:
  void setup    (const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream& stream) const;
  void uninstall(const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream& stream) const;
};

#endif
