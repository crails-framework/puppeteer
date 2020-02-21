#ifndef  VARIABLE_LIST_EDITOR_HPP
# define VARIABLE_LIST_EDITOR_HPP

# include <crails/front/custom_element.hpp>
# include <crails/front/mvc/view.hpp>
# include <crails/front/signal.hpp>
# include "app/models/variable_list.hpp"
# include "button.hpp"

namespace Views
{
  class VariableListEditor : public Crails::Front::CustomElement, public Crails::Listener
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
      std::cout << "Variable set_value #1" << std::endl;
      variables = value;
      render();
    }

    void set_value(const VariableList& value)
    {
      std::cout << "Variable set_value #2" << std::endl;
      variables = value;
      render();
    }

  private:
    void on_add_clicked(client::Event*);
    void on_change(client::Event*);
    void update_value();

    VariableList variables;
    std::list<std::pair<Crails::Front::Element,Crails::Front::Element> > inputs;
    Button button_add;
    bool read_only = false;

    std::list<std::shared_ptr<Crails::Front::JavascriptEvents> > event_listeners;
  };
}

#endif
