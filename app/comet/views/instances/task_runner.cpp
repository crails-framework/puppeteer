#include "task_runner.hpp"
#include "app/comet/sync_task.hpp"

extern Sync::Tasks* sync_tasks;

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
  {
    console_output->flush();
    console_output->visible(performing_action);
  }
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

Sync::TaskState TaskRunner::on_task_progress(Comet::Object response)
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
      if (response->hasOwnProperty("id"))
        sync_tasks->stop_listening((std::string)(response["id"]));
      on_action_performed();
      console_output->append_final_line("/!\\ Task aborted");
      return Sync::Abort;
    }
    else if (progress == 1)
    {
      if (response->hasOwnProperty("id"))
        sync_tasks->stop_listening((std::string)(response["id"]));
      on_action_performed();
      console_output->append_final_line("(!) Task successfully completed");
      return Sync::Success;
    }
  }
  return Sync::Continue;
}
