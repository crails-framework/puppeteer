#include "lib/odb/application-odb.hxx"
#include "builds.hpp"
#include "app/jenkins/jenkins.hpp"

using namespace std;
using namespace Crails;

BuildController::BuildController(Params& params) : Super(params)
{
}

void BuildController::enable()
{
  require_model();
  if (model)
  {
    Jenkins jenkins;
    int status = jenkins.enable_job(model->get_name());

    if (status >= 400)
      response["status"] = 500;
  }
}

void BuildController::disable()
{
  require_model();
  if (model)
  {
    Jenkins jenkins;
    int status = jenkins.disable_job(model->get_name());

    if (status >= 400)
      response["status"] = 500;
  }
}

void BuildController::builds()
{
  require_model();
  if (model)
  {
    Jenkins  jenkins;
    DataTree data = jenkins.get_project_data(model->get_name());

    response["body"] = data.as_data().to_json();
    model->update_last_build(data);
    database.save(*model);
  }
}
