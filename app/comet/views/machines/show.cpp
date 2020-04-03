#include <comet/http.hpp>
#include <crails/archive.hpp>
#include <crails/utils/string.hpp>
#include <chrono>
#include "show.hpp"
#include "app/comet/views/resources/elements/async_tasks.hpp"

using namespace std;
using namespace Comet;

extern Sync::Tasks* sync_tasks;
extern AsyncTasksWidget* async_tasks_widget;

Views::MachineShow::MachineShow()
{
  trigger_binding_updates();
  task_runner.set_console_output(&console_output);
  console_container.toggle_class("hidden", true);
}

void Views::MachineShow::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_machine(model->get_id());
    Breadcrumbs::done();
  }
}

static void format_free_return(string& memory)
{
  auto   parts = Crails::split(memory);
  auto   it    = parts.begin();
  string total, used;

  if (it != parts.end())
    total = *it;
  ++it;
  if (it != parts.end())
    used = *it;
  memory = used + " / " + total;
}

void Views::MachineShow::fetch_state()
{
  auto request = Http::Request::get(model->get_url() + "/state");

  request->set_headers({{"Accept", Archive::mimetype}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      IArchive archive;

      archive.set_data(response->get_response_text());
      archive & operating_system & hdd_space & memory;
      format_free_return(memory);
      signaler.trigger("state-fetched");
    }
  });
}

void Views::MachineShow::on_upgrade_clicked()
{
  auto request = Http::Request::post(model->get_url() + "/upgrade");

  task_runner.on_performing_action();
  console_container.toggle_class("hidden", false);
  view_container.toggle_class("performing-action", true);
  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      string task_uid = response->get_response_text();

      task_runner.task_listener_id =
        sync_tasks->listen_to(
          task_uid,
          std::bind(&MachineShow::on_upgrade_task_progress, this, std::placeholders::_1)
        );
      async_tasks_widget->add_task("Upgrading " + model->get_name(), task_uid);
    }
    else
      view_container.toggle_class("performing-action", false);
  });
}

void Views::MachineShow::on_upgrade_task_progress(Comet::Object response)
{
  switch (task_runner.on_task_progress(response))
  {
  case Sync::Success:
  case Sync::Abort:
    view_container.toggle_class("performing-action", false);
    task_runner.on_action_performed();
    model->set_last_upgrade(chrono::system_clock::to_time_t(chrono::system_clock::now()));
    signaler.trigger("upgraded");
    break ;
  case Sync::Continue:
    break ;
  }
}
