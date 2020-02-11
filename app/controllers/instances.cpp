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

void InstanceController::configure()
{
  require_model();
  if (model)
  {
    std::string task_uid;
    DataTree    task_params;

    task_params["id"] = model->get_id();
    task_uid = Sidekic::async_task("deploy_configuration", task_params.as_data());
    Crails::Controller::render(TEXT, task_uid);
    on_task_started("deploy_configuration", task_uid);
  }
}

void InstanceController::uninstall()
{
  require_model();
  if (model)
  {
    std::string task_uid;
    DataTree    task_params;

    task_params["id"] = model->get_id();
    task_uid = Sidekic::async_task("uninstall_configuration", task_params.as_data());
    Crails::Controller::render(TEXT, task_uid);
    on_task_started("uninstall_configuration", task_uid);
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
    model->get_build()->update_last_build();
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;
    Sync::Stream stream(output);
    InstanceState state;
    OArchive      archive;

    state.set_needs_restart(model->needs_restart());
    state.set_needs_configure(model->needs_configure());
    ssh.exec("monit status -g " + model->get_name(), stream);
    state.initialize_from_monit(output.str());
    state.serialize(archive);
    render(archive);
  });
}

void InstanceController::start()
{
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;
    Sync::Stream stream(output);

    ssh.exec("monit start -g " + model->get_name(), stream);
    model->set_last_start(model->get_build()->get_last_build());
    database.save(*model);
  });
}

void InstanceController::stop()
{
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;
    Sync::Stream stream(output);

    ssh.exec("monit stop -g " + model->get_name(), stream);
  });
}

void InstanceController::restart()
{
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;
    Sync::Stream stream(output);

    ssh.exec("monit restart -g " + model->get_name(), stream);
  });
}

void InstanceController::deploy()
{
  require_model();
  if (model)
  {
    std::string  task_uid;
    DataTree     task_params;
    auto         build    = model->get_build();
    unsigned int build_id = params["build_id"].defaults_to<unsigned int>(build->get_last_build());

    task_params["id"]       = model->get_id();
    task_params["build_id"] = build_id;
    task_uid = Sidekic::async_task("deploy_build", task_params.as_data());
    Crails::Controller::render(TEXT, task_uid);
    on_task_started("deploy_build", task_uid);
  }
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
