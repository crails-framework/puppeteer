#include "task_runner.hpp"

void TaskRunner::on_performing_action()
{
  performing_action = true;
  if (progress_bar)
  {
    progress_bar->set_active(true);
    progress_bar->update_progress(0);
  }
  else
    std::cout << "/!\\ TaskRunner::progress_bar unset" << std::endl;
  if (console_output)
    console_output->visible(performing_action);
  else
    std::cout << "/!\\ TaskRunner::console_output unset" << std::endl;
  performing_action_signal.trigger(true);
}

void TaskRunner::on_action_performed()
{
  performing_action = false;
  if (progress_bar)
    progress_bar->set_active(false);
  performing_action_signal.trigger(false);
}

Sync::TaskState TaskRunner::on_task_progress(Crails::Front::Object response)
{
  if (performing_action)
  {
    std::string status   = response->hasOwnProperty("status") ? (std::string)(response["status"]) : (std::string)("continue");
    float  progress      = client::parseFloat((const client::String*)(*response["progress"]));

    if (console_output && response->hasOwnProperty("message"))
      (*console_output) << (std::string)(response["message"]);
    if (progress_bar)
      progress_bar->update_progress(progress);
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
