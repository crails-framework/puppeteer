#include "instance.hpp"
#include "lib/odb/application-odb.hxx"
#include "app/ssh/session.hpp"
#include "app/ssh/channel.hpp"
#include "app/ssh/scp.hpp"

#include "recipe.hpp"

using namespace std;

odb_instantiable_impl(Instance)

void Instance::collect_variables(map<string,string>& variables) const
{
  variables["INSTANCE_NAME"] = get_name();
  variables["APP_USER"]      = get_user();
  variables["APP_PATH"]      = get_path();
}

void Instance::install()
{
  Recipe recipe;

  recipe.set_name("myrecipe");
  recipe.deploy_for(*this);
}
