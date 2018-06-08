#include "instance.hpp"
#include "lib/odb/application-odb.hxx"
#include "app/ssh/session.hpp"
#include "variable_list.hpp"
#include "recipe.hpp"
#include <crails/sync/task.hpp>

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

void Instance::configure(Sync::Task& task)
{
  auto recipe = get_build()->get_recipe();

  recipe->deploy_for(*this, task);
  set_state(Ready);
}

void Instance::uninstall(Sync::Task& task)
{
  auto recipe = get_build()->get_recipe();

  recipe->uninstall_from(*this, task);
  set_state(Uninstalled);
}

void Instance::open_ssh(std::function<void (Ssh::Session&)> callback)
{
  Ssh::Session ssh;

  ssh.should_accept_unknown_hosts(true);
  ssh.connect(Recipe::remote_user, get_machine()->get_ip());
  ssh.authentify_with_pubkey();
  callback(ssh);
}

bool Instance::needs_restart()
{
  return false;
}

bool Instance::needs_configure()
{
  return state == Uninstalled || state == Dirty;
}
