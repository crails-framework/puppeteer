#include <crails/router.hpp>
#include "lib/odb/application-odb.hxx"
#include "controllers/machines.hpp"
#include "controllers/instances.hpp"
#include "controllers/builds.hpp"
#include "controllers/recipes.hpp"
#include "controllers/credentials.hpp"
#include "controllers/backups.hpp"
#include "controllers/variable_sets.hpp"
#include "controllers/searches.hpp"
#include "controllers/dashboard.hpp"

void Crails::Router::initialize(void)
{
  auto& router = *this;

  crudify(router, "/machines",    MachineController);
  crudify(router, "/instances",   InstanceController);
  crudify(router, "/builds",      BuildController);
  crudify(router, "/recipes",     RecipeController);
  crudify(router, "/credentials", CredentialController);

  SetRoute("GET",    "/machines/:id/instances", InstanceController, by_machine);
  SetRoute("GET",    "/machines/:id/state",     MachineController,  fetch_state);
  SetRoute("POST",   "/machines/:id/upgrade",   MachineController,  upgrade);

  SetRoute("GET",    "/instances/:id/state",            InstanceController, fetch_state);
  SetRoute("GET",    "/instances/:id/logs",             InstanceController, fetch_logs);
  SetRoute("POST",   "/instances/:id/configure",        InstanceController, configure);
  SetRoute("POST",   "/instances/:id/deploy/:build_id", InstanceController, deploy);
  SetRoute("POST",   "/instances/:id/deploy",           InstanceController, deploy);
  SetRoute("POST",   "/instances/:id/uninstall",        InstanceController, uninstall);
  SetRoute("POST",   "/instances/:id/start",            InstanceController, start);
  SetRoute("POST",   "/instances/:id/stop",             InstanceController, stop);

  SetRoute("GET",    "/instances/:instance_id/backup",                 BackupController, show);
  SetRoute("POST",   "/instances/:instance_id/backup",                 BackupController, create_or_update);
  SetRoute("PUT",    "/instances/:instance_id/backup",                 BackupController, create_or_update);
  SetRoute("POST",   "/instances/:instance_id/backup/enable",          BackupController, enable);
  SetRoute("POST",   "/instances/:instance_id/backup/disable",         BackupController, disable);
  SetRoute("GET",    "/instances/:instance_id/backup/builds",          BackupController, builds);
  SetRoute("DELETE", "/instances/:instance_id/backup/builds/:number",  BackupController, remove_build);
  SetRoute("POST",   "/instances/:instance_id/backup/restore/:number", BackupController, restore);

  SetRoute("POST",   "/recipes/:id/fetch", RecipeController, fetch);

  SetRoute("GET",    "/builds/:id/builds",  BuildController, builds);
  SetRoute("POST",   "/builds/:id/enable",  BuildController, enable);
  SetRoute("POST",   "/builds/:id/disable", BuildController, disable);
  SetRoute("GET",    "/builds/:id/available-builds", BuildController, available_builds);
  SetRoute("DELETE", "/builds/:id/build/:build_id", BuildController, remove_build);

  SetRoute("GET",  "/variables", VariableSetController, show);
  SetRoute("POST", "/variables", VariableSetController, update);
  SetRoute("PUT",  "/variables", VariableSetController, update);

  SetRoute("GET", "/search/:query", SearchesController, search);

  SetRoute("GET", "/dashboard/last_builds", DashboardController, last_builds);
}
