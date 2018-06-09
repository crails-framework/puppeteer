#include "app/models/recipe.hpp"
#ifdef WITH_NGINX_PLUGIN
# include "modules/nginx/recipe_plugin.hpp"
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
  make_shared<EmptyPlugin>()
};
