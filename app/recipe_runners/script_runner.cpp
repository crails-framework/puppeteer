#include "script_runner.hpp"
#include "app/models/variable_set.hpp"
#include <crails/logger.hpp>

using namespace std;
using namespace Crails;

static string generate_variable_file(const map<string, string>& variables)
{
  stringstream stream;

  for (const auto& variable : variables)
    stream << "export " << variable.first << '=' << '"' << variable.second << '"' << '\n';
  return stream.str();
}

ScriptRunner::ScriptRunner(Ssh::Session& ssh, Recipe& recipe, Instance& instance, Sync::Task& task)
  : ssh(ssh), recipe(recipe), instance(instance), task(task), stream(task)
{
  task.set_task_count(4);
}

void ScriptRunner::require_remote_folder()
{
  if (remote_folder_created)
  {
    int status = ssh.exec("mkdir -p \"" + get_remote_folder() + '"', stream);

    if (status)
      std::runtime_error("could not create puppeteer's remote folder " + get_remote_folder());
    remote_folder_created = true;
  }
}

int ScriptRunner::run_script(const string& script_name)
{
  int status;

  status = ssh.exec("chmod +x '" + get_remote_folder() + '/' + script_name + ".sh" + '\'', stream);
  if (status)
    throw std::runtime_error("Recipe(" + recipe.get_name() + "): could not chmod script: deploy.sh");
  stream << "\n## " << script_name << ".sh:\n";

  stringstream command_stream;
  command_stream << "cd '" << get_remote_folder() << "' && " << "./" << script_name<< ".sh 2>&1";

  require_remote_folder();
  status = ssh.exec(command_stream.str(), stream);
  if (status && throw_on_failure_status)
  {
    stringstream err_stream;
    err_stream << "Recipe(" << recipe.get_name() << "): remote script `" << script_name << ".sh` returned with error status " << status;
    throw runtime_error(err_stream.str());
  }
  task.increment();
  return status;
}

void ScriptRunner::upload_script(const string& script_name)
{
  auto scp = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);

  stream << "Uploading " << script_name << " script...\n";
  require_remote_folder();
  scp->push_file(get_script_path(script_name), script_name + ".sh");
  stream << "Done.\n";
  task.increment();
}

void ScriptRunner::upload_variables()
{
  auto scp   = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);
  auto build = instance.get_build();

  VariableSet::collect_global_variables(variables);
  instance.collect_variables(variables);
  build->collect_variables(variables);
  require_remote_folder();
  scp->push_text(generate_variable_file(variables), get_variable_filepath());
  task.increment();
}

void ScriptRunner::cleanup()
{
  stringstream command;
  int status;

  stream << "Cleaning up...\n";
  command << "rm -Rf \"" << get_remote_folder() << '"';
  status = ssh.exec(command.str(), stream);
  if (status)
  {
    logger << Logger::Error << "ScriptRunner::cleanup: failed to clean up "
           << get_remote_folder() << " in "
           << instance.get_machine()->get_name() << Logger::endl;
  }
}
