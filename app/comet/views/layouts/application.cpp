#include "application.hpp"

AsyncTasksWidget* async_tasks_widget = nullptr;

ApplicationLayout::ApplicationLayout()
{
  async_tasks_widget = &tasks;
}

ApplicationLayout::~ApplicationLayout()
{
  async_tasks_widget = nullptr;
}
