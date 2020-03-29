#include <comet/http.hpp>
#include <iostream>
#include "app/comet/sync_task.hpp"
#include "_state_widget.hpp"

using namespace std;
using namespace Comet;

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
  auto request = Http::Request::post(model->get_url() + "/start");

  request->set_headers({{"Accept", "text/plain"}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();
    auto task_uid = response->get_response_text();

    on_performing_action();
    sync_tasks->listen_to(task_uid, std::bind(&InstanceStateWidget::on_start_task_progress, this, std::placeholders::_1));
  })._catch([]()
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

    on_performing_action();
    sync_tasks->listen_to(task_uid, std::bind(&InstanceStateWidget::on_stop_task_progress, this, std::placeholders::_1));
  })._catch([]()
  {
    std::cout << "STOP FAILED TO HAPPEN" << std::endl;
  });
}

void InstanceStateWidget::on_start_task_progress(Comet::Object response)
{
  switch (on_task_progress(response))
  {
  default:
    break;
  case Sync::Success:
    model->set_running(true);
    signaler.trigger("state-changed");
    break ;
  }
}

void InstanceStateWidget::on_stop_task_progress(Comet::Object response)
{
  switch (on_task_progress(response))
  {
  default:
    break;
  case Sync::Success:
    model->set_running(false);
    signaler.trigger("state-changed");
    break ;
  }
}

void InstanceStateWidget::fetch_state()
{
  auto request = Http::Request::get(model->get_url() + "/state");

  request->set_headers({{"Accept", "text/plain"}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();
    auto text     = response->get_response_text();

    model->set_running(text == "1");
    on_state_fetched();
  })._catch([]()
  {
    std::cout << "FAILED TO FETCH STATE" << std::endl;
  });
}

void InstanceStateWidget::on_state_fetched()
{
  state_fetched = true;
  signaler.trigger("state-changed");
}
