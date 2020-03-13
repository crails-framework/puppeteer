#ifndef  VARIABLE_LIST_EDITOR_HPP
# define VARIABLE_LIST_EDITOR_HPP

# include <comet/custom_element.hpp>
# include <comet/signal.hpp>
# include "app/models/variable_list.hpp"
# include "button.hpp"

namespace Views
{
  class VariableListEditor : public Comet::CustomElement, public Comet::Listener
  {
  public:
    VariableListEditor();

    void render();

    void activate(const VariableList& value)
    {
      set_value(value);
    }

    void set_read_only(bool value) { read_only = value; render(); }

    const VariableList& get_value() const { return variables; }

    void set_value(const std::string& value)
    {
      variables = value;
      render();
    }

    void set_value(const VariableList& value)
    {
      variables = value;
      render();
    }

  private:
    void on_add_clicked(client::Event*);
    void on_change(client::Event*);
    void update_value();

    VariableList variables;
    std::list<std::pair<Comet::Element,Comet::Element> > inputs;
    Button button_add;
    bool read_only = false;

    std::list<std::shared_ptr<Comet::JavascriptEvents> > event_listeners;
  };
}

#endif
