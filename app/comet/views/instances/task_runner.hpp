#ifndef  FRONT_TASK_RUNNER_HPP
# define FRONT_TASK_RUNNER_HPP

# include "_console_output.hpp"
# include "html/views/resources/elements/progress_bar.hpp"

namespace Sync
{
  enum TaskState
  {
    Continue = 0,
    Success  = 1,
    Abort    = 2
  };
}

struct TaskRunner
{
  Comet::Signal<bool> performing_action_signal;

  ConsoleOutput*             console_output = nullptr;
  HtmlTemplate::ProgressBar* progress_bar   = nullptr;
  bool performing_action = false;

  Sync::TaskState on_task_progress(Comet::Object);
  virtual void on_performing_action();
  virtual void on_action_performed();

  void set_console_output(ConsoleOutput* value) { console_output = value; }
  void set_progress_bar(HtmlTemplate::ProgressBar* value) { progress_bar = value; }
};

#endif
