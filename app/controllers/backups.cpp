#include "lib/odb/application-odb.hxx"
#include "backups.hpp"
#include "app/jenkins/jenkins.hpp"
#include <boost/filesystem.hpp>
#include <crails/sidekic.hpp>

using namespace std;
using namespace Crails;

BackupController::BackupController(Params& params) : Super(params)
{
}

void BackupController::create_or_update()
{
  if (find_model())
    update();
  else
    create();
}

bool BackupController::find_model()
{
  ODB::id_type instance_id = params["instance_id"];

  return database.find_one(model, odb::query<Backup>::instance_id == instance_id);
}

bool BackupController::initialize_model(Data data)
{
  ODB::id_type         instance_id = params["instance_id"];
  shared_ptr<Instance> instance;

  database.find_one(instance, odb::query<Instance>::id == instance_id);
  if (instance)
  {
    Super::initialize_model(data);
    model->set_instance(instance);
  }
  else
  {
    respond_with(ResponseStatus::not_found);
    return false;
  }
  return true;
}

void BackupController::after_create()
{
  model->on_change();
}

void BackupController::enable()
{
  require_model();
  if (model)
  {
    Jenkins jenkins;
    int status = jenkins.enable_job(model->get_name());

    if (status >= 400)
      response["status"] = 500;
  }
}

void BackupController::disable()
{
  require_model();
  if (model)
  {
    Jenkins jenkins;
    int status = jenkins.disable_job(model->get_name());

    if (status >= 400)
      response["status"] = 500;
  }
}

void BackupController::builds()
{
  require_model();
  if (model)
  {
    Jenkins  jenkins;
    DataTree data = jenkins.get_project_data(model->get_name());

    data["builds"].each([this](Data build) -> bool
    {
      auto number   = build["number"].as<string>();
      auto filename = model->get_tarball_path_for_backup(number);

      if (boost::filesystem::exists(filename))
      {
        build["success"] = true;
        build["timestamp"] = boost::filesystem::last_write_time(filename);
      }
      else
        build["success"] = false;
      return true;
    });
    response["body"] = data.as_data().to_json();
    database.save(*model);
  }
}

void BackupController::restore()
{
  require_model();
  if (model)
  {
    DataTree    task_params;
    std::string task_uid;

    task_params["backup_id"] = model->get_id();
    task_params["build_name"] = params["number"].as<string>();
    task_uid = Sidekic::async_task("restore", task_params.as_data());
    Crails::Controller::render(TEXT, task_uid);
  }
}

void BackupController::remove_build()
{
  require_model();
  if (model)
    model->remove_build(params["number"].as<unsigned int>());
}
