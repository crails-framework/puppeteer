#include "dashboard.hpp"
#include "app/comet/views/dashboard/index.hpp"

using namespace std;

DashboardController::DashboardController(const Comet::Params& p) : ApplicationController(p)
{
}

void DashboardController::index()
{
  auto view = make_shared<Views::Dashboard>();

  use_application_layout().render(view);
  view->activate();
}
