#include "show.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"
#include <iostream>

using namespace std;
using namespace Comet;

void Views::RecipeShow::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_recipe(model->get_id());
    Breadcrumbs::done();
  }
}

void Views::RecipeShow::on_pull_clicked()
{
  if (model)
  {
    auto request = Http::Request::post(model->get_url() + "/fetch");

    request->send().then([this, request]()
    {
      auto response = request->get_response();

      if (response->ok())
        on_fetched(*response);
      else
        on_fetch_failed(*response);
    });
  }
}

void Views::RecipeShow::on_fetched(const Comet::Http::Response&)
{
  std::cout << "recipe fetched" << std::endl;
  model->fetch().then([this]() { signaler.trigger("model-changed"); });
}

void Views::RecipeShow::on_fetch_failed(const Comet::Http::Response&)
{
  std::cout << "recipe fetch failed" << std::endl;
}
