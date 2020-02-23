#include <crails/front/http.hpp>
#include <iostream>
#include "front/app/sync_task.hpp"
#include "state_widget.hpp"

using namespace std;
using namespace Crails::Front;

extern Sync::Tasks* sync_tasks;

InstanceStateWidget::InstanceStateWidget()
{
}

void InstanceStateWidget::set_model(ModelPtr value)
{
  if (model != value)
  {
    model = value;
    if (model)
    {
      signaler.trigger("model-changed");
      fetch_state();
    }
  }
}

void InstanceStateWidget::on_start_clicked()
{
  auto request = Http::Request::post(model->get_url() + "/restart");

  request->set_headers({{"Accept", "text/plain"}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();
    auto task_uid = response->get_response_text();

    sync_tasks->listen_to(task_uid, std::bind(&InstanceStateWidget::on_start_task_progress, this, std::placeholders::_1));
    std::cout << "RESTARteD SUCESSFULLY RUN" << std::endl;
  })._catch([this]()
  {
    std::cout << "RESTART FAILED TO HAPPEN" << std::endl;
  });
}

void InstanceStateWidget::on_stop_clicked()
{
  auto request = Http::Request::post(model->get_url() + "/stop");

  request->set_headers({{"Accept", "text/plain"}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();
    auto task_uid = response->get_response_text();

    sync_tasks->listen_to(task_uid, std::bind(&InstanceStateWidget::on_stop_task_progress, this, std::placeholders::_1));
    std::cout << "STOP SUCCESSFULLY RUN" << std::endl;
  })._catch([this]()
  {
    std::cout << "STOP FAILED TO HAPPEN" << std::endl;
  });
}

void InstanceStateWidget::on_start_task_progress(Crails::Front::Object response)
{
  switch (on_task_progress(response))
  {
  default:
    break;
  }
}

void InstanceStateWidget::on_stop_task_progress(Crails::Front::Object response)
{
  switch (on_task_progress(response))
  {
  default:
    break;
  }
}

void InstanceStateWidget::fetch_state()
{
}

void InstanceStateWidget::on_state_fetched()
{
  signaler.trigger("state-changed");
}
