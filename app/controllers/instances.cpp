#include "instances.hpp"
#include "lib/odb/application-odb.hxx"

using namespace std;
using namespace Crails;

InstanceController::InstanceController(Params& params) : Super(params)
{
}

void InstanceController::configure()
{
  protect([this]()
  {
    require_model();
    if (model)
    {
      model->configure();
      database.save(*model);
    }
  });
}

void InstanceController::uninstall()
{
  protect([this]()
  {
    require_model();
    if (model)
    {
      model->uninstall();
      database.save(*model);
    }
  });
}

void InstanceController::protect(std::function<void()> callback)
{
  try {
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
