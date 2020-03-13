#include <comet/http.hpp>
#include "show.hpp"
#include "app/comet/views/resources/modal.hpp"
#include "html/views/backups/delete_backup_version.hpp"
#include "html/views/backups/restore.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"
#include "app/comet/time.hpp"
#include <ctime>
#include <iomanip>

using namespace std;
using namespace Comet;

void Views::BackupShow::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_instance(model->get_instance_id());
    Breadcrumbs::add_crumb("Backups", model->get_path());
    Breadcrumbs::done();
  }
  else
    std::cerr << "Cannot initialize breadcrumbs without a Backup model" << std::endl;
}

string Views::BackupShow::get_backup_date_string(Object data) const
{
  if (!data.is_undefined())
    return get_date_string((std::time_t)(data));
  return "N/A";
}

void Views::BackupShow::refresh_builds()
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

void Views::BackupShow::on_restore_build_clicked(string build_number)
{
  auto modal = Modal<HtmlTemplate::BackupRestore>::make("Restoring from backup " + build_number);

  modal->open().then([modal, build_number]()
  {
    if (modal->ok())
    {
      std::cout << "Backup restore not implemented yet" << std::endl;
    }
  });
}

void Views::BackupShow::on_remove_build_clicked(string build_number)
{
  auto modal = Modal<HtmlTemplate::DeleteBackupVersion>::make("Deleting backup " + build_number);

  modal->open().then([this, modal, build_number]()
  {
    if (modal->ok())
    {
      auto request = Http::Request::_delete(model->get_url() + "/builds/" + build_number);

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

void Views::BackupShow::set_enabled_state(bool value)
{
  set_is_enabled(value);
  signaler.trigger("enabled-state-changed");
}

void Views::BackupShow::on_enabled_clicked(bool value)
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
