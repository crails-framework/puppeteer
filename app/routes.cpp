#include <crails/router.hpp>
#include "controllers/machines.hpp"
#include "controllers/instances.hpp"
#include "controllers/builds.hpp"
#include "lib/odb/application-odb.hxx"

void Crails::Router::initialize(void)
{
  auto& router = *this;

  crudify(router, "/machines",  MachineController);
  crudify(router, "/instances", InstanceController);
  crudify(router, "/builds",    BuildController);

  SetRoute("GET", "/instances/:id/configure", InstanceController, configure);
}
