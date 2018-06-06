#ifndef  VIEW_INSTANCE_NEW_HPP
# define VIEW_INSTANCE_NEW_HPP

# include "utility/model_form.hpp"
# include "utility/select.hpp"
# include "utility/variable_list_editor.hpp"
# include "../app/instances.hpp"

namespace Views
{
  class InstanceNew : public ModelForm<Puppeteer::Instance>
  {
    Crails::Front::Element input_name, input_user;
    CollectionSelectWithName<Puppeteer::Machines> input_machine;
    CollectionSelectWithName<Puppeteer::Builds>   input_build;
    VariableListEditor input_variables;
  public:
    InstanceNew() : ModelForm("New instance")
    {
      input_name = El("input", {{"name","instance_name"},{"type","text"},{"class","form-control"}});
      input_user = El("input", {{"name","instance_user"},{"type","text"},{"class","form-control"}});
      input_machine.add_class("form-control");
      input_build.add_class("form-control");
    }

    std::unordered_map<std::string, El> get_inputs()
    {
      return {
        {"name",      input_name},
        {"user name", input_user},
        {"machine",   input_machine},
        {"build",     input_build},
        {"environment variables", input_variables.get_element()}
      };
    }

    void update_model_attributes()
    {
      model->set_name(input_name.get_value());
      model->set_user(input_user.get_value());
      model->set_machine_id( boost::lexical_cast<unsigned long>(input_machine.get_value()));
      model->set_build_id  ( boost::lexical_cast<unsigned long>(input_build.get_value()));
      model->set_variable_list(input_variables.get_value());
    }

    void update_form_attributes()
    {
      input_name.value(model->get_name());
      input_user.value(model->get_user());
      input_machine.value(boost::lexical_cast<std::string>(model->get_machine_id()));
      input_build.value  (boost::lexical_cast<std::string>(model->get_build_id()));
      input_variables.activate(model->get_variable_list());
    }

    void attached()
    {
      ModelForm::attached();
      input_machine.render();
      input_build.render();
      input_variables.render();
    }
  };
}

#endif
