#include <comet/router.hpp>
#include "controllers/builds.hpp"
#include "controllers/credentials.hpp"
#include "controllers/machines.hpp"
#include "controllers/instances.hpp"
#include "controllers/recipes.hpp"
#include "controllers/backups.hpp"
#include "controllers/variables.hpp"
#include "controllers/dashboard.hpp"
#include <comet/exception.hpp>

using namespace Comet;

static void workaround_router_unexplained_crash(Comet::Router* router)
{
  // Weirdly, if we call Crails::RouterBase::match before this code, it will crash.
  // This issue doesn't happen in all the builds of the application.
  // For now, it's only been seen on ubuntu:19.04 docker instances.
  Comet::Router::Item item;

  router->routes.push_back(item);
  router->routes.clear();
}

void Router::initialize()
{
  workaround_router_unexplained_crash(this);
  match_resource(BuildControllerDesc     ::resource_path, BuildsController);
  match_resource(CredentialControllerDesc::resource_path, CredentialsController);
  match_resource(MachineControllerDesc   ::resource_path, MachinesController);
  match_resource(InstanceControllerDesc  ::resource_path, InstancesController);
  match_resource(RecipeControllerDesc    ::resource_path, RecipesController);
  match_route("/instances/:id/backup",      BackupsController, show);
  match_route("/instances/:id/backup/edit", BackupsController, update);
  match_route("/variables", VariablesController, show);
  match_route("/?", DashboardController, index);
}
