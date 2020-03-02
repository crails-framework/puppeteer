#include "recipe.hpp"
#include "front/resources/elements/breadcrumbs.hpp"
#include <iostream>

using namespace std;

void Views::Recipe::initialize_breadcrumbs()
{
  if (model)
  {
    Breadcrumbs::reset();
    Breadcrumbs::set_recipe(model->get_id());
    Breadcrumbs::done();
  }
}

void Views::Recipe::on_pull_clicked()
{
  if (model)
  {
    Crails::Front::Ajax::query("POST", model->get_url() + "/fetch").callbacks({
      std::bind(&Recipe::on_fetched,      this, std::placeholders::_1),
      std::bind(&Recipe::on_fetch_failed, this, std::placeholders::_1)
})();
  }
}

void Views::Recipe::on_fetched(const Crails::Front::Ajax&)
{
  std::cout << "recipe fetched" << std::endl;
  model->fetch().then([this]() { signaler.trigger("model-changed"); });
}

void Views::Recipe::on_fetch_failed(const Crails::Front::Ajax&)
{
  std::cout << "recipe fetch failed" << std::endl;
}
