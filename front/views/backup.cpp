#include <crails/front/http.hpp>
#include "backup.hpp"
#include "resources/modal.hpp"

using namespace std;
using namespace Crails::Front;

void Views::Backup::refresh_builds()
{
  auto request = Http::Request::get(model->get_url() + "/builds");

  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      DataTree data = response->get_response_data();

      set_enabled_state(data["buildable"].as<string>() == "true");
      builds = data.as_data().as_object()["builds"];
      signaler.trigger("builds-fetched");
    }
    else
      std::cout << "Failed to fetch builds for backup " << model->get_id() << std::endl;
  });
}

void Views::Backup::on_restore_build_clicked(string build_number)
{
}

void Views::Backup::on_remove_build_clicked(string build_number)
{
}

void Views::Backup::set_enabled_state(bool value)
{
  set_is_enabled(value);
  signaler.trigger("enabled-state-changed");
}

void Views::Backup::on_enabled_clicked(bool value)
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
