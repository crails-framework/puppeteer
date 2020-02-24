#include "instance.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "app/ssh/session.hpp"
# include "app/recipe_runners/script_runner.hpp"
# include <crails/logger.hpp>
#endif
#include "variable_list.hpp"
#include "recipe.hpp"

using namespace std;
using namespace Crails;

odb_instantiable_impl(Instance)

#ifndef __CHEERP_CLIENT__
void Instance::collect_variables(map<string,string>& variables)
{
  const VariableList local_variables = get_variables();

  variables["INSTANCE_NAME"] = get_name();
  variables["HOST_IP"]       = get_machine()->get_ip();
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

void Instance::deploy(Sync::Task& task, const std::string& build_id)
{
  auto recipe = get_build()->get_recipe();

  recipe->deploy_build_for(*this, task, build_id);
  set_state(Ready);
}

void Instance::open_ssh(std::function<void (Ssh::Session&)> callback)
{
  Ssh::Session ssh;

  ssh.should_accept_unknown_hosts(true);
  ssh.connect(Recipe::remote_user, get_machine()->get_ip());
  ssh.authentify_with_pubkey();
  callback(ssh);
}

void Instance::start(Sync::Task& task)
{
  auto build  = get_build();
  auto recipe = build->get_recipe();

  recipe->exec_script("start", *this, task);
}

void Instance::stop(Sync::Task& task)
{
  auto build  = get_build();
  auto recipe = build->get_recipe();

  recipe->exec_script("stop", *this, task);
}

void Instance::update_running_state(Sync::Task& task)
{
  open_ssh([this, &task](Ssh::Session& ssh)
  {
    const string script_name = "state";
    auto         recipe = get_build()->get_recipe();
    ScriptRunner runner(ssh, *recipe, *this, task);
    int          status;

    runner.throw_on_failure_status = false;
    runner.upload_variables();
    runner.upload_script(script_name);
    status = runner.run_script(script_name);
    switch (status)
    {
    case 0:
      set_running(true);
      break ;
    case -1:
      set_state(Instance::Dirty);
    case 1:
      set_running(false);
      break ;
    default:
      logger << Logger::Error << "state script for instance " << get_name() << " returned an unknown status (" << status << ')' << Logger::endl;
      break ;
    }
  });
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
