#include "async_tasks.hpp"
#include "app/comet/sync_task.hpp"
#include <comet/globals.hpp>

using namespace std;

extern Sync::Tasks* sync_tasks;

AsyncTasksWidget::AsyncTasksWidget() : Comet::CustomElement("ul")
{
  attr("class", "async-tasks");
  bind_attributes();
}

void AsyncTasksWidget::add_task(const std::string& name,  const std::string& uid)
{
  Task* task = new Task;
  std::string uid_copy = uid;

  task->label = Comet::Element("label").text(name);
  task->progress_bar.set_active(true);
  task->progress_bar.update_progress(0);
  task->listener_id = sync_tasks->listen_to(
    uid,
    std::bind(
      &AsyncTasksWidget::on_task_progress,
      this,
      uid_copy,
      placeholders::_1
    )
  );
  tasks.emplace(uid, task);
  render_tasks();
}

void AsyncTasksWidget::render_tasks()
{
  html("");
  for (auto& task : tasks)
  {
    Comet::Element item("li");
    item > task.second->label;
    item > task.second->progress_bar;
    *this > item;
  }
}

void AsyncTasksWidget::on_task_progress(std::string uid, Comet::Object response)
{
  auto task = tasks.find(uid);

  if (task != tasks.end())
  {
    auto&  progress_bar = task->second->progress_bar;
    string status = response->hasOwnProperty("status")
      ? (string)(response["status"])
      : (string)("continue");
    float progress = client::parseFloat(
      (const client::String*)(*response["progress"])
    );

    std::cout << "[AsyncTasksWidget][" << uid << "] progress: " << (progress * 100) << '%' << std::endl;
    progress_bar.update_progress(progress);
    if (status == "abort")
      on_task_done(uid, false);
    else if (progress == 1)
      on_task_done(uid, true);
    else
      progress_bar.trigger_binding_updates();
  }
}

void AsyncTasksWidget::on_task_done(const std::string& uid, bool success)
{
  auto task = tasks.find(uid);

  if (task != tasks.end())
  {
    auto& progress_bar = task->second->progress_bar;

    progress_bar.set_theme(success ? "success" : "warning");
    progress_bar.trigger_binding_updates();
    remove_task(uid);
    sync_tasks->stop_listening(task->second->listener_id);
  }
}

void AsyncTasksWidget::remove_task(const std::string& uid)
{
  Comet::window.set_timeout([this, uid]()
  {
    auto  it   = tasks.find(uid);
    Task* task = it->second;

    delete task;
    tasks.erase(it);
    render_tasks();
  }, 5000);
}
