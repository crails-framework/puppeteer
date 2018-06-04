#include <crails/router.hpp>
#include "controllers/machines.hpp"
#include "controllers/instances.hpp"
#include "controllers/builds.hpp"
#include "controllers/recipes.hpp"
#include "lib/odb/application-odb.hxx"

void Crails::Router::initialize(void)
{
  auto& router = *this;

  crudify(router, "/machines",  MachineController);
  crudify(router, "/instances", InstanceController);
  crudify(router, "/builds",    BuildController);
  crudify(router, "/recipes",   RecipeController);

  SetRoute("GET",  "/instances/:id/state",     InstanceController, fetch_state);
  SetRoute("POST", "/instances/:id/configure", InstanceController, configure);
  SetRoute("POST", "/instances/:id/uninstall", InstanceController, uninstall);
  SetRoute("POST", "/instances/:id/start",     InstanceController, restart);
  SetRoute("POST", "/instances/:id/stop",      InstanceController, stop);
}
