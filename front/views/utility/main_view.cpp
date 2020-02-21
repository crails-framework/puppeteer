#include <crails/front/globals.hpp>
#include "main_view.hpp"
#include "template_view.hpp"
#include <crails/front/template_element.hpp>

using namespace client;

MainView::MainView() : view(nullptr)
{
  std::cout << "MainView constructor" << std::endl;
  Crails::Front::body.empty().inner({*this});
  bind_attributes();
  trigger_binding_updates();
}

void MainView::attach(Crails::Front::Element& new_view)
{
  auto item = std::make_shared<Crails::Front::CustomElement>();

  item->inner({new_view});
  detach_current_view();
  slot_content.set_element(item);
  //new_view.attached();
  view = &new_view;
}

void MainView::detach_current_view()
{
  if (view)
  {
    //view->detached();
    //view->destroy();
    delete view;
  }
}
