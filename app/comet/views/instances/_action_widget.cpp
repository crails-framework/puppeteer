#include "_action_widget.hpp"
#include "app/comet/sync_task.hpp"
#include <comet/http.hpp>
#include <comet/lexical_cast.hpp>
#include "app/comet/collections/builds.hpp"
#include "app/comet/views/resources/elements/async_tasks.hpp"
#include <iostream>

using namespace Comet;
//using namespace Views;
using namespace std;

extern Sync::Tasks* sync_tasks;
extern AsyncTasksWidget* async_tasks_widget;

InstanceActionWidget::InstanceActionWidget()
{
  set_progress_bar(&progress_bar_el);
}

bool InstanceActionWidget::can_deploy() const
{
  return model
      && model->get_state() > 0
      && model->get_build_id() != ODB_NULL_ID;
}

void InstanceActionWidget::fetch_build_versions()
{
  auto url     = Front::Build::get_available_builds_url_for(model->get_build_id());
  auto request = Http::Request::get(url);

  request->set_headers({{"Accept","application/json"}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      DataTree data_tree = response->get_response_data();

      set_build_versions(data_tree.as_data());
      signaler.trigger("builds-fetched");
    }
  })._catch([]()
  {
    std::cout << "Failed to fetch available build versions" << std::endl;
  });
}

Actions InstanceActionWidget::get_visible_actions()
{
  Actions actions;

  if (model)
  {
    actions.push_back(Action("cloud-upload-alt", "Configure", std::bind(&InstanceActionWidget::configure, this)));
    if (model->get_state() > 0)
      actions.push_back(Action("eraser", "Uninstall", std::bind(&InstanceActionWidget::uninstall, this)));
  }
  return actions;
}

void InstanceActionWidget::set_model(const std::shared_ptr<Front::Instance>& instance)
{
  HtmlTemplate::ActionWidget::set_model(instance);
  signaler.trigger("model-changed");
  if (model)
  {
    if (model->get_build_id() != ODB_NULL_ID)
      fetch_build_versions();
    listen_to(model->remote_state_changed, std::bind(&InstanceActionWidget::on_remote_state_changed, this));
    on_remote_state_changed();
  }
}

void InstanceActionWidget::on_remote_state_changed()
{
  signaler.trigger("actions-changed");
}

void InstanceActionWidget::configure()
{
  if (!performing_action)
  {
    auto request = Http::Request::post(model->get_url() + "/configure");

    on_performing_action();
    request->send().then([this, request]()
    {
      auto response = request->get_response();

      if (response->ok())
        on_configure_start(*response);
      else
        on_configure_failure(*response);
    });
  }
}

void InstanceActionWidget::uninstall()
{
  if (!performing_action)
  {
    auto request = Http::Request::post(model->get_url() + "/uninstall");

    on_performing_action();
    request->send().then([this, request]()
    {
      auto response = request->get_response();

      if (response->ok())
        on_uninstall_start(*response);
      else
        on_uninstall_failure(*response);
    });
  }
}

std::string InstanceActionWidget::get_selected_build_version()
{
  auto checked_options = build_version_select.find("option:checked");

  if (checked_options.size() > 0)
    return checked_options[0].get_value();
  return "";
}

void InstanceActionWidget::deploy()
{
  std::cout << "InstanceActionWidget::deploy" << std::endl;
  if (!performing_action)
  {
    std::string url = model->get_url() + "/deploy";
    std::string build_version = get_selected_build_version();

    if (build_version.length() > 0)
    {
      url += '/' + build_version;
      deploying_build = build_version;
    }
    on_performing_action();
    auto request = Http::Request::post(url);
    request->send().then([this, request]()
    {
      auto response = request->get_response();

      if (response->ok())
        on_deploy_start(*response);
      else
        on_deploy_failure(*response);
    });
  }
  std::cout << "InstanceActionWidget::deploy launched" << std::endl;
}

vector<Element> InstanceActionWidget::get_buttons()
{
  return find(".actions > button");
}

void InstanceActionWidget::on_performing_action()
{
  TaskRunner::on_performing_action();
  for (auto button : get_buttons())
    button.attr("disabled", "disabled");
}

void InstanceActionWidget::on_action_performed()
{
  TaskRunner::on_action_performed();
  for (auto button : get_buttons())
    button->removeAttribute("disabled");
}

void InstanceActionWidget::on_uninstall_start(const Comet::Http::Response& response)
{
  std::string task_uid = response.get_response_text();

  progress_bar->set_label("Uninstalling...");
  progress_bar->update_progress(0);
  task_listener_id =
    sync_tasks->listen_to(task_uid, std::bind(&InstanceActionWidget::on_uninstall_task_progress, this, std::placeholders::_1));
  async_tasks_widget->add_task("Uninstalling " + model->get_name(), task_uid);
}

void InstanceActionWidget::on_uninstall_task_progress(Comet::Object response)
{
  switch (on_task_progress(response))
  {
  default:
  case Sync::Abort:
    break ;
  case Sync::Success:
    model->set_state(Instance::Uninstalled);
    model->remote_state_changed.trigger();
    break ;
  }
}

void InstanceActionWidget::on_deploy_start(const Comet::Http::Response& response)
{
  std::string task_uid = response.get_response_text();

  progress_bar->set_label("Deployment...");
  progress_bar->update_progress(0);
  task_listener_id =
    sync_tasks->listen_to(task_uid, std::bind(&InstanceActionWidget::on_deploy_task_progress, this, std::placeholders::_1));
  async_tasks_widget->add_task("Deploying " + model->get_name(), task_uid);
}

void InstanceActionWidget::on_deploy_failure(const Comet::Http::Response&)
{
  std::cout << "Deploy query failed" << std::endl;
}

void InstanceActionWidget::on_deploy_task_progress(Comet::Object response)
{
  switch (on_task_progress(response))
  {
  case Sync::Success:
    model->set_state(Instance::Deployed);
    if (deploying_build.length() > 0)
      model->set_deployed_build(Comet::lexical_cast<unsigned int>(deploying_build));
    model->remote_state_changed.trigger();
    break ;
  case Sync::Abort:
  case Sync::Continue:
    break ;
  }
}

void InstanceActionWidget::on_configure_start(const Comet::Http::Response& response)
{
  std::string task_uid = response.get_response_text();

  progress_bar->set_label("Configure...");
  progress_bar->update_progress(0);
  task_listener_id =
    sync_tasks->listen_to(task_uid, std::bind(&InstanceActionWidget::on_configure_task_progress, this, std::placeholders::_1));
  async_tasks_widget->add_task("Configuring " + model->get_name(), task_uid);
}

void InstanceActionWidget::on_configure_task_progress(Comet::Object response)
{
  switch (on_task_progress(response))
  {
  case Sync::Success:
    model->set_state(Instance::Ready);
    model->remote_state_changed.trigger();
    break ;
  case Sync::Abort:
  case Sync::Continue:
    break ;
  }
}

void InstanceActionWidget::on_uninstall_failure(const Comet::Http::Response&)
{
  std::cout << "uninstall failure" << std::endl;
  model->set_state(2);
  model->remote_state_changed.trigger();
}

void InstanceActionWidget::on_configure_failure(const Comet::Http::Response&)
{
  std::cout << "configure failure" << std::endl;
  model->set_state(2);
  model->remote_state_changed.trigger();
}
