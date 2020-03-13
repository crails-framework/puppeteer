#include <comet/http.hpp>
#include <crails/utils/string.hpp>
#include "show.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"
#include "app/comet/views/resources/modal.hpp"
#include "html/views/builds/delete_build_version.hpp"

using namespace std;
using namespace Comet;

void Views::BuildShow::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_build(model->get_id());
    Breadcrumbs::done();
  }
}

std::string Views::BuildShow::get_badge_for_build(std::string number)
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

void Views::BuildShow::refresh_builds()
{
 auto request = Http::Request::get(model->get_url() + "/builds");

 request->send().then([this, request]()
 {
   auto response = request->get_response();

   if (response->ok())
     on_builds_fetched(*response);
   else
     on_builds_fetch_failed(*response);
 });
}

void Views::BuildShow::on_builds_fetched(const Http::Response& response)
{
  auto data = response.get_response_object();

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

void Views::BuildShow::on_remove_build_clicked(string build_number)
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

void Views::BuildShow::set_enabled_state(bool value)
{
  set_is_enabled(value);
  signaler.trigger("enabled-state-changed");
}

void Views::BuildShow::on_enable_build_clicked(bool value)
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
