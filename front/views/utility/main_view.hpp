#ifndef  FRONT_MAIN_VIEW_HPP
# define FRONT_MAIN_VIEW_HPP

# include <crails/front/element.hpp>
# include "lib/cheerp-html/layout.hpp"

class TemplateView;

class MainView : public HtmlTemplate::Layout
{
public:
  MainView();

  template<typename VIEW_TYPE>
  void attach(std::shared_ptr<VIEW_TYPE> new_view)
  {
    detach_current_view();
    slot_content.set_element(new_view);
    new_view->bind_attributes();
    new_view->trigger_binding_updates();
  }

  void on_route_executed()
  {
    searchInput.reset();
  }

  static MainView* instance;

private:
  void detach_current_view();
};

#endif
