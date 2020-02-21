#ifndef  VARIABLE_SET_NEW_HPP
# define VARIABLE_SET_NEW_HPP

# include "utility/model_form.hpp"
# include "../app/variable_sets.hpp"
# include "lib/cheerp-html/views/variable_set_new.hpp"

namespace Views
{
  class VariableSetForm : public ModelForm<Puppeteer::VariableSet, HtmlTemplate::VariableSetNew>
  {
  public:
    ~VariableSetForm()
    {
      std::cout << "Destroying variable set form" << std::endl;
    }

    std::string get_title() const { return "Global variables"; }

    void activate()
    {
      variables_input.render();
      ModelForm::activate(0);
    }

    void set_model(std::shared_ptr<Puppeteer::VariableSet> _model)
    {
      ModelForm::set_model(_model);
      variables_input.set_value(model->get_value());
      variables_input.render();
    }

    void update_model_attributes()
    {
      model->set_value(variables_input.get_value());
    }
  };
}

#endif
