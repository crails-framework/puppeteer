#include <crails/front/globals.hpp>
#include "main_view.hpp"
#include "template_view.hpp"
#include <crails/front/custom_element.hpp>

using namespace client;

MainView::MainView()
{
  std::cout << "MainView constructor" << std::endl;
  Crails::Front::body.empty().inner({*this});
  bind_attributes();
  trigger_binding_updates();
}

void MainView::detach_current_view()
{
}
