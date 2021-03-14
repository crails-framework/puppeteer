#ifndef  VIEWS_BUTTON_HPP
# define VIEWS_BUTTON_HPP

# include <comet/element.hpp>
# include <comet/signal.hpp>

namespace Views
{
  class Button : public Comet::Element
  {
  public:
    Comet::Signal<client::Event*> clicked;

    Button() : Comet::Element("button")
    {
      add_class("btn");
      listener = cheerp::Callback([this](client::Event* event) { clicked.trigger(event); });
      operator*()->addEventListener("click", listener);
    }

    virtual ~Button()
    {
      operator*()->removeEventListener("click", listener);
    }

  private:
    client::EventListener* listener;
  };
}

#endif
