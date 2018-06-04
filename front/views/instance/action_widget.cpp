#include "action_widget.hpp"
#include <iostream>

using namespace Views;
using namespace std;

InstanceActionWidget::InstanceActionWidget()
{
  button_deploy.add_class("btn-warning");
  button_deploy.text("Deploy");
  listen_to(button_deploy.clicked, std::bind(&InstanceActionWidget::deploy, this, std::placeholders::_1));
}

void InstanceActionWidget::activate(std::shared_ptr<Puppeteer::Instance> instance)
{
  model = instance;
  inner({ button_deploy });
}

void InstanceActionWidget::deploy(client::Event*)
{
  Crails::Front::Ajax::query("POST", model->get_url() + "/configure").callbacks({
    std::bind(&InstanceActionWidget::on_deployed,       this, std::placeholders::_1),
    std::bind(&InstanceActionWidget::on_deploy_failure, this, std::placeholders::_1)
  })();
}

void InstanceActionWidget::on_deployed(const Crails::Front::Ajax&)
{
  std::cout << "deploy success" << std::endl;
}

void InstanceActionWidget::on_deploy_failure(const Crails::Front::Ajax&)
{
  std::cout << "deploy failure" << std::endl;
}
