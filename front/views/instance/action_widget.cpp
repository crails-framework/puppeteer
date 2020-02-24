#include "action_widget.hpp"
#include "front/app/sync_task.hpp"
#include "front/views/utility/theme.hpp"
#include <crails/front/http.hpp>
#include "app/builds.hpp"
#include <iostream>

using namespace Crails::Front;
using namespace Views;
using namespace std;

extern Sync::Tasks* sync_tasks;

InstanceActionWidget::InstanceActionWidget()
{
  progress_bar = &progress_bar_el;
}

bool InstanceActionWidget::can_deploy() const
{
  return model && model->get_state() > 0;
}

void InstanceActionWidget::fetch_build_versions()
{
  auto url     = Puppeteer::Build::get_available_builds_url_for(model->get_build_id());
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
  })._catch([this]()
  {
    std::cout << "Failed to fetch available build versions" << std::endl;
  });
}

Actions InstanceActionWidget::get_visible_actions()
{
  Actions actions;

  if (model)
  {
    if (model->get_state() == 0)
      actions.push_back(Action("cloud-upload-alt", "Configure", std::bind(&InstanceActionWidget::configure, this)));
    if (model->get_state() > 0)
      actions.push_back(Action("eraser", "Uninstall", std::bind(&InstanceActionWidget::uninstall, this)));
  }
  return actions;
}

void InstanceActionWidget::set_model(std::shared_ptr<Puppeteer::Instance> instance)
{
  HtmlTemplate::ActionWidget::set_model(instance);
  signaler.trigger("model-changed");
  if (model)
  {
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
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/configure").callbacks({
      std::bind(&InstanceActionWidget::on_configure_start,     this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_configure_failure,   this, std::placeholders::_1)
    })();
  }
}

void InstanceActionWidget::uninstall()
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/uninstall").callbacks({
      std::bind(&InstanceActionWidget::on_uninstall_start,     this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_uninstall_failure,   this, std::placeholders::_1)
    })();
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
      url += '/' + build_version;
    on_performing_action();
    Crails::Front::Ajax::query("POST", url).callbacks({
      std::bind(&InstanceActionWidget::on_deploy_start,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_deploy_failure, this, std::placeholders::_1)
    })();
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

void InstanceActionWidget::on_uninstall_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output->flush();
  progress_bar->set_label("Uninstalling...");
  progress_bar->set_progress(0);
  sync_tasks->listen_to(task_uid, std::bind(&InstanceActionWidget::on_uninstall_task_progress, this, std::placeholders::_1));
}

void InstanceActionWidget::on_uninstall_task_progress(Crails::Front::Object response)
{
  switch (on_task_progress(response))
  {
  default:
    break ;
  case Sync::Success:
    model->set_state(1);
    model->remote_state_changed.trigger();
    break ;
  case Sync::Abort:
    model->set_state(2);
    model->remote_state_changed.trigger();
    break ;
  }
}

void InstanceActionWidget::on_deploy_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output->flush();
  progress_bar->set_label("Deployment...");
  progress_bar->set_progress(0);
  sync_tasks->listen_to(task_uid, std::bind(&InstanceActionWidget::on_deploy_task_progress, this, std::placeholders::_1));
}

void InstanceActionWidget::on_deploy_failure(const Crails::Front::Ajax& ajax)
{
  std::cout << "Deploy query failed" << std::endl;
}

void InstanceActionWidget::on_deploy_task_progress(Crails::Front::Object response)
{
  switch (on_task_progress(response))
  {
  case Sync::Success:
    model->set_state(Instance::Deployed);
    model->set_deployed_build(boost::lexical_cast<unsigned int>(deploying_build));
    model->remote_state_changed.trigger();
    break ;
  case Sync::Abort:
  case Sync::Continue:
    break ;
  }
}

void InstanceActionWidget::on_configure_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output->flush();
  progress_bar->set_label("Configure...");
  progress_bar->set_progress(0);
  sync_tasks->listen_to(task_uid, std::bind(&InstanceActionWidget::on_configure_task_progress, this, std::placeholders::_1));
}

void InstanceActionWidget::on_configure_task_progress(Crails::Front::Object response)
{
  switch (on_task_progress(response))
  {
  case Sync::Success:
    model->set_state(0);
    model->remote_state_changed.trigger();
    break ;
  case Sync::Abort:
    model->set_state(2);
    model->remote_state_changed.trigger();
    break ;
  case Sync::Continue:
    break ;
  }
}

void InstanceActionWidget::on_uninstall_failure(const Crails::Front::Ajax&)
{
  std::cout << "uninstall failure" << std::endl;
  model->set_state(2);
  model->remote_state_changed.trigger();
}

void InstanceActionWidget::on_configure_failure(const Crails::Front::Ajax&)
{
  std::cout << "configure failure" << std::endl;
  model->set_state(2);
  model->remote_state_changed.trigger();
}
