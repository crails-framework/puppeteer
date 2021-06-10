#include "instance.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "app/ssh/session.hpp"
# include "app/recipe_runners/script_runner.hpp"
# include <crails/logger.hpp>
# include <chrono>
#endif
#include "variable_list.hpp"
#include "recipe.hpp"

using namespace std;
using namespace Crails;

odb_instantiable_impl(Instance)

shared_ptr<Recipe> Instance::get_instance_recipe()
{
  if (get_build_id() != ODB_NULL_ID)
    return get_build()->get_recipe();
  return get_recipe();
}

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
  auto recipe = get_instance_recipe();

  recipe->deploy_for(*this, task);
  if (state == Uninstalled)
    set_state(Ready);
}

void Instance::uninstall(Sync::Task& task)
{
  auto recipe = get_instance_recipe();

  recipe->uninstall_from(*this, task);
  set_state(Uninstalled);
}

void Instance::deploy(Sync::Task& task, const std::string& build_id)
{
  auto recipe = get_instance_recipe();

  recipe->deploy_build_for(*this, task, build_id);
  set_state(Deployed);

  stringstream stream; unsigned int a;
  stream << build_id; stream >> a;
  set_deployed_build(a);
}

void Instance::open_ssh(std::function<void (Ssh::Session&)> callback)
{
  get_machine()->open_ssh(callback);
}

void Instance::start(Sync::Task& task)
{
  auto build  = get_build();
  auto recipe = get_instance_recipe();

  recipe->exec_script("start", *this, task);
  last_start = chrono::system_clock::to_time_t(chrono::system_clock::now()); 
}

void Instance::stop(Sync::Task& task)
{
  auto build  = get_build();
  auto recipe = get_instance_recipe();

  recipe->exec_script("stop", *this, task);
}

void Instance::update_running_state(Sync::Task& task)
{
  open_ssh([this, &task](Ssh::Session& ssh)
  {
    const string script_name = "state";
    auto         recipe = get_instance_recipe();
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
    auto recipe = get_instance_recipe();

    return recipe->get_last_tip() != get_last_configure();
  }
  return true;
}

bool Instance::can_destroy() const
{
  return state == Uninstalled;
}

void Instance::before_destroy()
{
}
#endif
