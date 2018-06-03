#include "main_view.hpp"
#include "template_view.hpp"

using namespace client;

MainView::MainView() : view(0)
{
  el = Crails::Front::Element(client::document.getElementById("page-container"));
}

void MainView::attach(TemplateView& new_view)
{
  detach_current_view();
  new_view.emplace(el);
  new_view.attached();
  view = &new_view;
}

void MainView::detach_current_view()
{
  if (view)
  {
    view->detached();
    delete view;
  }
}
