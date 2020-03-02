#include <crails/front/http.hpp>
#include <crails/utils/string.hpp>
#include "front/resources/elements/breadcrumbs.hpp"
#include "build.hpp"
#include "resources/modal.hpp"
#include "lib/cheerp-html/views/delete/delete_build_version.hpp"

using namespace std;
using namespace Crails::Front;

void Views::Build::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_build(model->get_id());
    Breadcrumbs::done();
  }
}

std::string Views::Build::get_badge_for_build(std::string number)
{
  stringstream stream;
  vector<string> parts;

  for (auto part : Crails::split(job_url, '/'))
    parts.push_back(part);
  stream << parts.at(0) << "//" << parts.at(1)
         << "/buildStatus/icon?job=" << parts.at(3) << "%2F" << parts.at(5)
         << "&build=" << number;
  return stream.str();
}

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
    job_url = (std::string)(data["url"]);
    builds = data["builds"];
    signaler.trigger("builds-fetched");
  }
  else
    std::cout << "fetched builds, but there's no json response" << std::endl;
}

void Views::Build::on_remove_build_clicked(string build_number)
{
  auto modal = Modal<HtmlTemplate::DeleteBuildVersion>::make("Deleting build " + build_number);

  modal->open().then([this, modal, build_number]()
  {
    if (modal->ok())
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
  });
}

void Views::Build::set_enabled_state(bool value)
{
  set_is_enabled(value);
  signaler.trigger("enabled-state-changed");
}

void Views::Build::on_enable_build_clicked(bool value)
{
  string action  = value ? "enable" : "disable";
  auto   request = Http::Request::post(model->get_url() + '/' + action);

  request->send().then([this, request, value]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      set_enabled_state(value);
      std::cout << "Successfully changed enable state" << std::endl;
    }
    else
      std::cout << "Failed to change enable state" << std::endl;
  });
}
