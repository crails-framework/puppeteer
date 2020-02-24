#include "lib/odb/application-odb.hxx"
#include "builds.hpp"
#include "app/jenkins/jenkins.hpp"
#include <crails/utils/string.hpp>

using namespace std;
using namespace Crails;

BuildController::BuildController(Params& params) : Super(params)
{
}

void BuildController::after_create()
{
  model->on_change();
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

void BuildController::available_builds()
{
  require_model();
  if (model)
  {
    auto filenames = model->get_available_builds();
    stringstream json;

    json << '[';
    for (auto it = filenames.begin() ; it != filenames.end() ; ++it)
    {
      if (it != filenames.begin())
        json << ',';
      json << '"' << (*it) << '"';
    }
    json << ']';
    response["body"] = json.str();
  }
}
