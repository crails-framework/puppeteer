#include "instance.hpp"
#include "lib/odb/application-odb.hxx"
#include "app/ssh/session.hpp"
#include "app/ssh/channel.hpp"
#include "app/ssh/scp.hpp"
#include "variable_list.hpp"
#include "recipe.hpp"

using namespace std;

odb_instantiable_impl(Instance)

void Instance::collect_variables(map<string,string>& variables) const
{
  const VariableList local_variables = get_variables();

  variables["INSTANCE_NAME"] = get_name();
  variables["APP_USER"]      = get_user();
  variables["APP_PATH"]      = get_path() == "" ? ("/home/" + get_user()) : get_path();
  local_variables.to_map(variables);
}

void Instance::configure()
{
  auto recipe = get_build()->get_recipe();

  recipe->deploy_for(*this);
  set_state(Ready);
}

void Instance::uninstall()
{
  auto recipe = get_build()->get_recipe();

  recipe->uninstall_from(*this);
  set_state(Uninstalled);
}
