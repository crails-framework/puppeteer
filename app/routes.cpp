#include <crails/router.hpp>
#include "lib/odb/application-odb.hxx"
#include "controllers/machines.hpp"
#include "controllers/instances.hpp"
#include "controllers/builds.hpp"
#include "controllers/recipes.hpp"
#include "controllers/credentials.hpp"
#include "controllers/variable_sets.hpp"

void Crails::Router::initialize(void)
{
  auto& router = *this;

  crudify(router, "/machines",    MachineController);
  crudify(router, "/instances",   InstanceController);
  crudify(router, "/builds",      BuildController);
  crudify(router, "/recipes",     RecipeController);
  crudify(router, "/credentials", CredentialController);

  SetRoute("GET",  "/instances/:id/state",     InstanceController, fetch_state);
  SetRoute("POST", "/instances/:id/configure", InstanceController, configure);
  SetRoute("POST", "/instances/:id/deploy",    InstanceController, deploy);
  SetRoute("POST", "/instances/:id/uninstall", InstanceController, uninstall);
  SetRoute("POST", "/instances/:id/start",     InstanceController, restart);
  SetRoute("POST", "/instances/:id/restart",   InstanceController, restart);
  SetRoute("POST", "/instances/:id/stop",      InstanceController, stop);

  SetRoute("POST", "/recipes/:id/fetch", RecipeController, fetch);

  SetRoute("GET",  "/builds/:id/builds",  BuildController, builds);
  SetRoute("POST", "/builds/:id/enable",  BuildController, enable);
  SetRoute("POST", "/builds/:id/disable", BuildController, disable);

  SetRoute("GET",  "/variables", VariableSetController, show);
  SetRoute("POST", "/variables", VariableSetController, update);
  SetRoute("PUT",  "/variables", VariableSetController, update);
}
