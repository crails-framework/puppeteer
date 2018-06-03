#ifndef  VARIABLE_LIST_EDITOR_HPP
# define VARIABLE_LIST_EDITOR_HPP

# include <crails/front/mvc/view.hpp>
# include "app/models/variable_list.hpp"
# include "button.hpp"

namespace Views
{
  class VariableListEditor : public Crails::Front::View
  {
  public:
    VariableListEditor();

    void render();

    void activate(const VariableList& value)
    {
      variables = value;
      render();
    }

    void set_read_only(bool value) { read_only = value; render(); }

    const VariableList& get_value() const { return variables; }

  private:
    void on_add_clicked(client::Event*);
    void on_change(client::Event*);
    void update_value();

    VariableList variables;
    std::list<std::pair<El,El> > inputs;
    Button button_add;
    bool read_only = false;
  };
}

#endif
