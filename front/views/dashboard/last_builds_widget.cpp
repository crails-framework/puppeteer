#include "last_builds_widget.hpp"
#include "config/jenkins.hpp"
#include <crails/front/http.hpp>

using namespace std;
using namespace Crails::Front;

void LastBuildsWidget::fetch_last_builds()
{
  auto request = Http::Request::get("/dashboard/last_builds");

  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      last_builds.clear();
      last_builds_response = response->get_response_data();
      if (last_builds_response["jobs"].exists())
      {
        aggregate_builds_from_jobs(last_builds_response["jobs"]);
        signaler.trigger("last-builds-changed");
      }
      else
        std::cerr << "/dashboard/last_builds query returned no jobs" << std::endl;
    }
  });
}

void LastBuildsWidget::aggregate_builds_from_jobs(Data jobs)
{
  // Aggregate the builds
  last_builds_response["jobs"].each([this](Data job_data) -> bool
  {
    const string job_name = job_data["name"];

    job_data["builds"].each([this, job_name](Data build_data) -> bool
    {
      build_data["name"] = job_name;
      last_builds.push_back(build_data);
      return true;
    });
    return true;
  });
  // Sort the builds
  sort(last_builds.begin(), last_builds.end(), [](Data a, Data b)
  {
    return a["timestamp"].as<double>() > b["timestamp"].as<double>();
  });
  // Enforce the list size limit
  last_builds.resize(LAST_BUILDS_LIST_SIZE);
}
