#include "action_widget.hpp"
#include "front/app/sync_task.hpp"
#include "front/views/utility/theme.hpp"
#include <iostream>

using namespace Views;
using namespace std;

typedef Crails::Front::Element El;

extern Sync::Tasks* sync_tasks;

InstanceActionWidget::InstanceActionWidget(ConsoleOutput& console_output) : console_output(console_output)
{
  button_configure.inner({ Theme::fa_icon("cloud-upload-alt"), El("span").text(" Configure") });
  button_deploy   .inner({ Theme::fa_icon("cloud-upload-alt"), El("span").text(" Deploy") });
  button_restart  .inner({ Theme::fa_icon("play"),             El("span").text(" Start") });
  button_stop     .inner({ Theme::fa_icon("stop"),             El("span").text(" Stop") });
  button_uninstall.inner({ Theme::fa_icon("eraser"),           El("span").text(" Uninstall") });

  for (auto& button : vector<Button*>({ &button_configure, &button_deploy, &button_restart, &button_stop, &button_uninstall }))
    button->add_class("btn-primary");

  listen_to(button_configure.clicked, std::bind(&InstanceActionWidget::configure, this, std::placeholders::_1));
  listen_to(button_uninstall.clicked, std::bind(&InstanceActionWidget::uninstall, this, std::placeholders::_1));
  listen_to(button_restart.clicked,   std::bind(&InstanceActionWidget::restart,   this, std::placeholders::_1));
  listen_to(button_stop.clicked,      std::bind(&InstanceActionWidget::stop,      this, std::placeholders::_1));
}

void InstanceActionWidget::activate(std::shared_ptr<Puppeteer::Instance> instance)
{
  model = instance;
  listen_to(model->remote_state_changed, std::bind(&InstanceActionWidget::render, this));
  render();
}

void InstanceActionWidget::render()
{
  html("");
  attr("style","text-align:center");
  inner({
    El("div", {{"class","btn-group"}}).inner({
      button_deploy, button_restart, button_stop, button_uninstall
    }),
    El("div", {{"class","progress mb-3"},{"style","margin-top:20px"}}).inner({progress_bar})
  });
  button_configure.visible(model->get_state() != 1 || model->state.get_needs_configure());
  button_uninstall.visible(model->get_state()  > 0);
  button_deploy   .visible(model->get_state()  > 0);
  button_restart  .visible(model->get_state() != 0);
  button_stop     .visible(model->get_state() != 0);
}

void InstanceActionWidget::configure(client::Event*)
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

void InstanceActionWidget::uninstall(client::Event*)
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

void InstanceActionWidget::deploy(client::Event*)
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/deploy").callbacks({
      std::bind(&InstanceActionWidget::on_deploy_start,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_deploy_failure, this, std::placeholders::_1)
    })();
  }
}

void InstanceActionWidget::restart(client::Event*)
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

void InstanceActionWidget::stop(client::Event*)
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

const vector<El*> InstanceActionWidget::get_buttons()
{
  return {&button_configure, &button_uninstall, &button_deploy, &button_restart, &button_stop};
}

void InstanceActionWidget::on_performing_action()
{
  performing_action = true;
  for (auto* button : get_buttons())
    button->attr("disabled","disabled");
  progress_bar.set_active(true);
  console_output.visible(performing_action);
}

void InstanceActionWidget::on_action_performed()
{
  performing_action = false;
  for (auto* button : get_buttons())
    (*button)->removeAttribute("disabled");
  progress_bar.set_active(false);
}

void InstanceActionWidget::on_uninstall_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output.flush();
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
      console_output << (std::string)(response["message"]);
    progress_bar.set_progress(progress);
    if (status == "abort")
    {
      on_action_performed();
      console_output << "/!\\ Task aborted\n";
      return Sync::Abort;
    }
    else if (progress == 1)
    {
      on_action_performed();
      console_output << "(!) Task successfully completed\n";
      return Sync::Success;
    }
  }
  return Sync::Continue;
}

void InstanceActionWidget::on_configure_start(const Crails::Front::Ajax& ajax)
{
  std::string task_uid = ajax.get_response_text();

  console_output.flush();
  progress_bar.set_progress(0);
  progress_bar.text("Deployment...");
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
