#ifndef  VARIABLE_SET_NEW_HPP
# define VARIABLE_SET_NEW_HPP

# include "../utility/model_form.hpp"
# include "app/comet/models/variable_set.hpp"
# include "html/views/variable_sets/new.hpp"

namespace Views
{
  class VariableSetForm : public ModelForm<Front::VariableSet, HtmlTemplate::VariableSetNew>
  {
  public:
    VariableSetForm();
    virtual ~VariableSetForm();


    std::string get_title() const { return "Global variables"; }

    void activate();
    void set_model(std::shared_ptr<Front::VariableSet> _model);
    void initialize_breadcrumbs();
    void update_model_attributes();
  };
}

#endif
