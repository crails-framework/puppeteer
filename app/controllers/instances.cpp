#include "instances.hpp"
#include "lib/odb/application-odb.hxx"
#include "app/ssh/session.hpp"
#include "app/models/instance_state.hpp"

using namespace std;
using namespace Crails;

InstanceController::InstanceController(Params& params) : Super(params)
{
}

void InstanceController::configure()
{
  require_model();
  protect([this]()
  {
    if (model)
    {
      model->configure();
      database.save(*model);
    }
  });
}

void InstanceController::uninstall()
{
  require_model();
  protect([this]()
  {
    if (model)
    {
      model->uninstall();
      database.save(*model);
    }
  });
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
    InstanceState state;
    OArchive      archive;

    ssh.exec("monit status -g " + model->get_name(), output);
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

    ssh.exec("monit start -g " + model->get_name(), output);
  });
}

void InstanceController::stop()
{
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;

    ssh.exec("monit stop -g " + model->get_name(), output);
  });
}

void InstanceController::restart()
{
  open_ssh([this](Ssh::Session& ssh)
  {
    stringstream output;

    ssh.exec("monit restart -g " + model->get_name(), output);
  });
}

void InstanceController::protect(std::function<void()> callback)
{
  try {
    callback();
  } catch (...) {
    if (model)
    {
      model->set_state(Instance::Dirty);
      database.save(*model);
      database.commit();
      throw ;
    }
  }
}
