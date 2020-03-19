#include "application.hpp"
#include "faye.hpp"
#include "sync_task.hpp"

Sync::Faye*  faye;
Sync::Tasks* sync_tasks;

void Application::start()
{
  auto& app = get();

  faye       = new Sync::Faye;
  sync_tasks = new Sync::Tasks(*faye);
  app.router.start();
}
