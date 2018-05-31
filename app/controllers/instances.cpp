#include "instances.hpp"
#include "lib/odb/application-odb.hxx"

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
    model->install();
  }
}
