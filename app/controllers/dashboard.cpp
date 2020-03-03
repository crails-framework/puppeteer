#include "dashboard.hpp"
#include "app/jenkins/jenkins.hpp"

using namespace std;
using namespace Crails;

DashboardController::DashboardController(Params& params) : Super(params)
{
}

void DashboardController::last_builds()
{
  Jenkins jenkins;
  DataTree data = jenkins.get_last_builds();

  response["body"] = data.as_data().to_json();
}
