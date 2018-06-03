#ifndef  VIEWS_BUTTON_HPP
# define VIEWS_BUTTON_HPP

# include <crails/front/element.hpp>
# include <crails/front/signal.hpp>

namespace Views
{
  class Button : public Crails::Front::Element
  {
  public:
    Crails::Signal<client::Event*> clicked;

    Button() : Crails::Front::Element("button")
    {
      add_class("btn");
      listener = cheerp::Callback([this](client::Event* event) { clicked.trigger(event); });
      operator*()->addEventListener("click", listener);
    }

    ~Button()
    {
      operator*()->removeEventListener("click", listener);
    }

  private:
    client::EventListener* listener;
  };
}

#endif
