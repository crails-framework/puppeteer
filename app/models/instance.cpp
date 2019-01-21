#include "instance.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "app/ssh/session.hpp"
# include <crails/sync/task.hpp>
#endif
#include "variable_list.hpp"
#include "recipe.hpp"

using namespace std;

odb_instantiable_impl(Instance)

#ifndef __CHEERP_CLIENT__
void Instance::collect_variables(map<string,string>& variables) const
{
  const VariableList local_variables = get_variables();

  variables["INSTANCE_NAME"] = get_name();
  variables["APP_USER"]      = get_user();
  variables["APP_PATH"]      = path == "" ? ("/home/" + get_user()) : path;
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
  return get_last_start() <= get_build()->get_last_build();
}

bool Instance::needs_configure()
{
  if (state == Ready)
  {
    auto recipe = get_build()->get_recipe();

    return recipe->get_last_tip() != get_last_configure();
  }
  return true;
}
#endif
