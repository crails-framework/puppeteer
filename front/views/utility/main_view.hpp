#ifndef  FRONT_MAIN_VIEW_HPP
# define FRONT_MAIN_VIEW_HPP

# include <crails/front/element.hpp>

class TemplateView;

class MainView
{
public:
  MainView();

  void attach(TemplateView&);

  static MainView* instance;

private:
  void detach_current_view();

  Crails::Front::Element el;
  TemplateView* view;
};

#endif
