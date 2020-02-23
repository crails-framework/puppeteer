#include "action_widget.hpp"
#include "front/app/sync_task.hpp"
#include "front/views/utility/theme.hpp"
#include <iostream>

using namespace Views;
using namespace std;

typedef Crails::Front::Element El;

extern Sync::Tasks* sync_tasks;

InstanceActionWidget::InstanceActionWidget()
{
}

Actions InstanceActionWidget::get_visible_actions()
{
  Actions actions;

  if (model)
  {
    if (model->get_state() != 1)
      actions.push_back(Action("cloud-upload-alt", "Configure", std::bind(&InstanceActionWidget::configure, this)));
    if (model->get_state() > 0)
      actions.push_back(Action("cloud-upload-alt", "Deploy", std::bind(&InstanceActionWidget::deploy, this)));
    if (model->get_state() != 0)
      actions.push_back(Action("play", "Start", std::bind(&InstanceActionWidget::restart, this)));
    if (model->get_state() != 0)
      actions.push_back(Action("stop", "Stop", std::bind(&InstanceActionWidget::stop, this)));
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

void InstanceActionWidget::deploy()
{
  std::cout << "InstanceActionWidget::deploy" << std::endl;
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/deploy").callbacks({
      std::bind(&InstanceActionWidget::on_deploy_start,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_deploy_failure, this, std::placeholders::_1)
    })();
  }
  std::cout << "InstanceActionWidget::deploy launched" << std::endl;
}

void InstanceActionWidget::restart()
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/restart")
	    .headers({{"Accept","text/plain"}})
	    .callbacks({
      std::bind(&InstanceActionWidget::on_restarted,        this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_restart_failed,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_ajax_action_performed, this, std::placeholders::_1)
    })();
  }
}

void InstanceActionWidget::stop()
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/stop").callbacks({
      std::bind(&InstanceActionWidget::on_stopped,          this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_stop_failed,      this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_ajax_action_performed, this, std::placeholders::_1)
    })();
  }
}

std::vector<Crails::Front::Element> InstanceActionWidget::get_buttons()
{
  return find(".actions > button");
}

void InstanceActionWidget::on_performing_action()
{
  performing_action = true;
  for (auto button : get_buttons())
    button.attr("disabled", "disabled");
  progress_bar.set_active(true);
  console_output->visible(performing_action);
}

void InstanceActionWidget::on_action_performed()
{
  performing_action = false;
  for (auto button : get_buttons())
    button->removeAttribute("disabled");
  progress_bar.set_active(false);
}

void InstanceActionWidget::on_uninstall_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output->flush();
  progress_bar.set_progress(0);
  progress_bar.text("Uninstalling...");
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

Sync::TaskState InstanceActionWidget::on_task_progress(Crails::Front::Object response)
{
  if (performing_action)
  {
    std::string status   = response->hasOwnProperty("status") ? (std::string)(response["status"]) : (std::string)("continue");
    float  progress      = client::parseFloat((const client::String*)(*response["progress"]));

    if (response->hasOwnProperty("message"))
      (*console_output) << (std::string)(response["message"]);
    progress_bar.set_progress(progress);
    if (status == "abort")
    {
      on_action_performed();
      (*console_output) << "/!\\ Task aborted\n";
      return Sync::Abort;
    }
    else if (progress == 1)
    {
      on_action_performed();
      (*console_output) << "(!) Task successfully completed\n";
      return Sync::Success;
    }
  }
  return Sync::Continue;
}

void InstanceActionWidget::on_deploy_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output->flush();
  progress_bar.set_progress(0);
  progress_bar.text("Deployment...");
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
    //model->set_state(0);
    //model->remote_state_changed.trigger();
    break ;
  case Sync::Abort:
    //model->set_state(2);
    //model->remote_state_changed.trigger();
    break ;
  case Sync::Continue:
    break ;
  }
}

void InstanceActionWidget::on_configure_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output->flush();
  progress_bar.set_progress(0);
  progress_bar.text("Configure...");
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

void InstanceActionWidget::on_restarted(const Crails::Front::Ajax&)
{
  model->remote_state_changed.trigger();
}

void InstanceActionWidget::on_stopped(const Crails::Front::Ajax&)
{
  model->remote_state_changed.trigger();
}

void InstanceActionWidget::on_restart_failed(const Crails::Front::Ajax&)
{
  std::cout << "instance start failure" << std::endl;
}

void InstanceActionWidget::on_stop_failed(const Crails::Front::Ajax&)
{
  std::cout << "instance stop failure" << std::endl;
}
