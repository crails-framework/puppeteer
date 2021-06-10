#include "lib/odb/application-odb.hxx"
#include "instances.hpp"
#include "app/ssh/session.hpp"
#include "app/models/instance_state.hpp"
#include <crails/sidekic.hpp>

using namespace std;
using namespace Crails;

InstanceController::InstanceController(Params& params) : Super(params)
{
}

void InstanceController::by_machine()
{
  Query query = Query::machine_id == params["id"].as<ODB::id_type>();
  odb::result<Instance> results;

  //paginator.decorate_query(query);
  database.find(results, query);
  models = odb::to_vector<Instance>(results);
  paginator.decorate_view(vars, [this, query]() {
    return database.count<Instance>(query);
  });
  vars["models"] = &models;
  render(get_index_view());
}

void InstanceController::configure()
{
  DataTree task_params;

  schedule_task("deploy_configuration", task_params.as_data());
}

void InstanceController::uninstall()
{
  DataTree task_params;

  schedule_task("uninstall_configuration", task_params.as_data());
}

void InstanceController::start()
{
  DataTree task_params;

  task_params["script_name"] = "start";
  schedule_task("instance_script", task_params.as_data());
}

void InstanceController::stop()
{
  DataTree task_params;

  task_params["script_name"] = "stop";
  schedule_task("instance_script", task_params.as_data());
}

void InstanceController::deploy()
{
  require_model();
  if (model)
  {
    DataTree     task_params;
    auto         build    = model->get_build();
    unsigned int build_id = params["build_id"].defaults_to<unsigned int>(build->get_last_build());

    task_params["build_id"] = build_id;
    schedule_task("deploy_build", task_params.as_data());
  }
}

void InstanceController::schedule_task(const std::string& task_name, Data task_params)
{
  require_model();
  if (model)
  {
    std::string task_uid;

    task_params["id"] = model->get_id();
    task_uid = Sidekic::async_task(task_name, task_params);
    Crails::Controller::render(TEXT, task_uid);
    on_task_started(task_name, task_uid);
  }
}

void InstanceController::open_ssh(function<void(Ssh::Session&)> callback)
{
  require_model();
  if (model)
    model->open_ssh(callback);
}

void InstanceController::fetch_state()
{
  require_model();
  if (model)
  {
    Sync::Task foo_task(3);
    string response_text;

    model->update_running_state(foo_task);
    response_text = model->get_running() ? "1" : "0";
    Crails::Controller::render(TEXT, response_text);
  }
}

void InstanceController::fetch_logs()
{
  OArchive     archive;
  unsigned int last_line_count = params["last_count"].defaults_to<unsigned int>(0);
  unsigned int line_count = 0;
  string       text;

  require_model();
  if (!model)
    return ;
  if (model->get_running_task() == "")
  {
    auto recipe = model->get_instance_recipe();

    if (recipe)
      recipe->fetch_logs(*model, last_line_count, line_count, text);
    else
      line_count = last_line_count;
  }
  else
    line_count = last_line_count;
  archive & line_count & text;
  render(archive);
}

void InstanceController::on_task_started(const string& task_name, const string& task_uid)
{
  if (model)
  {
    model->set_running_task(task_name + ':' + task_uid);
    database.save(*model);
    database.commit();
  }
}
