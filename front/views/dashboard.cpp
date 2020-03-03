#include "dashboard.hpp"
#include "front/resources/elements/breadcrumbs.hpp"
#include <crails/front/http.hpp>

using namespace std;
using namespace Crails::Front;

void Views::Dashboard::initialize_breadcrumbs()
{
  Breadcrumbs::reset();
  Breadcrumbs::done();
}

void Views::Dashboard::activate()
{
  initialize_breadcrumbs();
  fetch_last_builds();
}

void Views::Dashboard::fetch_last_builds()
{
  auto request = Http::Request::get("/dashboard/last_builds");

  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      last_builds.clear();
      last_builds_response = response->get_response_data();
      last_builds_response.as_data().each([this](Data job_data) -> bool
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
      signaler.trigger("last-builds-changed");
    }
  });
}
