#include "instances.hpp"
#include "lib/odb/application-odb.hxx"
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
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;
    Sync::Stream stream(output);
    InstanceState state;
    OArchive      archive;

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
