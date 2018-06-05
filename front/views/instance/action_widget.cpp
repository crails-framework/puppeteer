#include "action_widget.hpp"
#include <iostream>

using namespace Views;
using namespace std;

typedef Crails::Front::Element El;

InstanceActionWidget::InstanceActionWidget()
{
  button_deploy   .text("Deploy")   .add_class("btn-warning");
  button_restart  .text("Restart")  .add_class("btn-primary");
  button_stop     .text("Stop")     .add_class("btn-danger");
  button_uninstall.text("Uninstall").add_class("btn-danger");
  listen_to(button_deploy.clicked,    std::bind(&InstanceActionWidget::deploy,    this, std::placeholders::_1));
  listen_to(button_uninstall.clicked, std::bind(&InstanceActionWidget::uninstall, this, std::placeholders::_1));
  listen_to(button_restart.clicked,   std::bind(&InstanceActionWidget::restart,   this, std::placeholders::_1));
  listen_to(button_stop.clicked,      std::bind(&InstanceActionWidget::stop,      this, std::placeholders::_1));
}

void InstanceActionWidget::activate(std::shared_ptr<Puppeteer::Instance> instance)
{
  model = instance;
  render();
}

void InstanceActionWidget::render()
{
  attr("style","text-align:center");
  inner({
    El("div", {{"class","btn-group"}}).inner({
      button_deploy, button_uninstall, button_restart, button_stop
    }),
    
  });
  button_deploy   .visible(model->get_state() != 1);
  button_uninstall.visible(model->get_state()  > 0);
  button_restart  .visible(model->get_state() != 0);
  button_stop     .visible(model->get_state() != 0);
}

void InstanceActionWidget::deploy(client::Event*)
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/configure").callbacks({
      std::bind(&InstanceActionWidget::on_deployed,         this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_deploy_failure,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_action_performed, this, std::placeholders::_1)
    })();
  }
}

void InstanceActionWidget::uninstall(client::Event*)
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/uninstall").callbacks({
      std::bind(&InstanceActionWidget::on_deployed,         this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_deploy_failure,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_action_performed, this, std::placeholders::_1)
    })();
  }
}

void InstanceActionWidget::restart(client::Event*)
{
  if (!performing_action)
  {
    on_performing_action();
    Crails::Front::Ajax::query("POST", model->get_url() + "/restart").callbacks({
      std::bind(&InstanceActionWidget::on_restarted,        this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_restart_failed,   this, std::placeholders::_1),
      std::bind(&InstanceActionWidget::on_action_performed, this, std::placeholders::_1)
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
      std::bind(&InstanceActionWidget::on_action_performed, this, std::placeholders::_1)
    })();
  }
}

const vector<El*> InstanceActionWidget::get_buttons()
{
  return {&button_deploy, &button_uninstall, &button_restart, &button_stop};
}

void InstanceActionWidget::on_performing_action()
{
  performing_action = true;
  for (auto* button : get_buttons())
    button->attr("disabled","disabled");
}

void InstanceActionWidget::on_action_performed(const Crails::Front::Ajax&)
{
  performing_action = false;
  for (auto* button : get_buttons())
    (*button)->removeAttribute("disabled");
}

void InstanceActionWidget::on_deployed(const Crails::Front::Ajax&)
{
  std::cout << "deploy success" << std::endl;
  model->set_state(0);
}

void InstanceActionWidget::on_deploy_failure(const Crails::Front::Ajax&)
{
  std::cout << "deploy failure" << std::endl;
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
