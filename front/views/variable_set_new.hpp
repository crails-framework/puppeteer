#ifndef  VARIABLE_SET_NEW_HPP
# define VARIABLE_SET_NEW_HPP

# include "utility/model_form.hpp"
# include "../app/variable_sets.hpp"

namespace Views
{
  class VariableSetForm : public ModelForm<Puppeteer::VariableSet>
  {
    VariableListEditor input_variables;
  public:
    VariableSetForm() : ModelForm("Global variables")
    {
    }

    std::unordered_map<std::string, El> get_inputs()
    {
      return {
        {"environment variables", input_variables}
      };
    }

    void update_model_attributes()
    {
      model->set_value(input_variables.get_value());
    }

    void update_form_attributes()
    {
      input_variables.activate(model->get_value());
    }

    void attached()
    {
      ModelForm::attached();
      input_variables.render();
    }

  private:
  };
}

#endif
