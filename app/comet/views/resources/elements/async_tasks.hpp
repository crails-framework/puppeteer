#ifndef  ASYNC_TASKS_WIDGET_HPP
# define ASYNC_TASKS_WIDGET_HPP

# include <comet/custom_element.hpp>
# include "lib/comet/html/views/resources/elements/progress_bar.hpp"
# include "app/comet/sync_task.hpp"

class AsyncTasksWidget : public Comet::CustomElement
{
  struct Task
  {
    Comet::Element            label;
    HtmlTemplate::ProgressBar progress_bar;
    Sync::Tasks::ListenerId   listener_id;
  };

  std::map<std::string, Task*> tasks;

public:
  AsyncTasksWidget();

  void add_task(const std::string& name,  const std::string& uid);
  void render_tasks();

  void on_task_progress(std::string uid, Comet::Object response);
  void on_task_done(const std::string& uid, bool success);
  void remove_task(const std::string& uid);
};

#endif
