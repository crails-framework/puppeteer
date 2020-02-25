#include <crails/front/http.hpp>
#include "build.hpp"

using namespace std;
using namespace Crails::Front;

void Views::Build::refresh_builds()
{
  Ajax::query("GET", model->get_url() + "/builds").callbacks({
    std::bind(&Build::on_builds_fetched,      this, std::placeholders::_1),
    std::bind(&Build::on_builds_fetch_failed, this, std::placeholders::_1)
  })();
}

void Views::Build::on_builds_fetched(const Ajax& ajax)
{
  auto data = ajax.get_response_as_json();

  if (!data.is_undefined())
  {
    std::cout << "fetched builds" << std::endl;
    std::string buildable = data["buildable"];

    set_enabled_state(buildable == "true");
    builds = data["builds"];
    signaler.trigger("builds-fetched");
  }
  else
    std::cout << "fetched builds, but there's no json response" << std::endl;
}

void Views::Build::on_remove_build_clicked(string build_number)
{
  auto request = Http::Request::_delete(model->get_url() + "/build/" + build_number);

  request->send().then([this, request, build_number]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      std::cout << "Removed build " << build_number << std::endl;
      refresh_builds();
    }
    else
    {
      std::cout << "Failed to remove build " << build_number << std::endl;
    }
  });
}

void Views::Build::set_enabled_state(bool value)
{
  string action  = value ? "enable" : "disable";
  auto   request = Http::Request::post(model->get_url() + '/' + action);

  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      std::cout << "Successfully changed enable state" << std::endl;
    }
    else
      std::cout << "Failed to change enable state" << std::endl;
  });
}
