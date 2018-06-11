#include <crails/getenv.hpp>
#include "app/models/recipe.hpp"

#ifdef WITH_NGINX_PLUGIN
# include "modules/nginx/recipe_plugin.hpp"
#endif

#ifdef WITH_GANDI_PLUGIN
# include "modules/gandi/recipe_plugin.hpp"
const int         GandiPlugin::domain_zone_id = Crails::getenv_as<int>("GANDI_DOMAIN_ZONE_ID", 0);
const std::string GandiPlugin::domain_name    = Crails::getenv("GANDI_DOMAIN_NAME");
#endif

using namespace std;

struct EmptyPlugin : public Recipe::Plugin
{
  bool recipe_uses_plugin(const std::string& recipe_folder) const { return false; }
  void apply(const std::string&, const std::string&, Instance&, const std::map<std::string,std::string>&, Sync::Stream&) const {}
};

const vector<shared_ptr<Recipe::Plugin> > Recipe::plugins = {
#ifdef WITH_NGINX_PLUGIN
  make_shared<NginxPlugin>(),
#endif
#ifdef WITH_GANDI_PLUGIN
  make_shared<GandiPlugin>(),
#endif
  make_shared<EmptyPlugin>()
};
