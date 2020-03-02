#include "instance.hpp"
#include "../app/backups.hpp"
#include "front/resources/elements/breadcrumbs.hpp"

using namespace std;

Views::Instance::Instance()
{
  state_widget.set_progress_bar(action_widget.progress_bar);
  action_widget.set_console_output(&console_output);
  state_widget .set_console_output(&console_output);
  action_widget.performing_action_signal.connect(std::bind(&Instance::on_action_state_update, this, std::placeholders::_1));
  state_widget .performing_action_signal.connect(std::bind(&Instance::on_action_state_update, this, std::placeholders::_1));
}

void Views::Instance::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_instance(model->get_id());
    Breadcrumbs::done();
  }
}

void Views::Instance::activate(unsigned long instance_id)
{
  ModelView::activate(instance_id);
}

void Views::Instance::on_model_received()
{
  ModelView::on_model_received();
  model->remote_state_changed.connect([this]()
  {
    signaler.trigger("state-changed");
  });
}

void Views::Instance::on_action_state_update(bool is_busy)
{
  auto buttons = find(".actions .au-btn, .build-deploy-ui .au-btn");

  for (auto button : buttons)
    button.toggle_class("disabled", is_busy);
}

string Views::Instance::get_backup_path() const
{
  return Puppeteer::Backup::get_path_for_instance(model->get_id());
}
