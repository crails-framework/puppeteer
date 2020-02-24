#ifndef  VIEW_INSTANCE_NEW_HPP
# define VIEW_INSTANCE_NEW_HPP

# include "utility/model_form.hpp"
# include "utility/select.hpp"
# include "utility/variable_list_editor.hpp"
# include "../app/instances.hpp"

# include "lib/cheerp-html/views/instance_new.hpp"

namespace Views
{
  class InstanceNew : public ModelForm<Puppeteer::Instance, HtmlTemplate::InstanceNew>
  {
  public:
    std::string   get_title() const { return model ? model->get_name() : "New instance"; }
    std::string   get_instance_name() const       { return model ? model->get_name() : ""; }
    std::string   get_instance_user() const       { return model ? model->get_user() : ""; }
    unsigned long get_instance_build_id() const   { return model ? model->get_build_id() : ODB_NULL_ID; }
    unsigned long get_instance_machine_id() const { return model ? model->get_machine_id() : ODB_NULL_ID; }
    std::string   get_instance_variables() const  { return model ? model->get_variables() : ""; }

    void update_model_attributes()
    {
      auto name_input = find("[name=\"instance_name\"]")[0];
      auto user_input = find("[name=\"instance_user\"]")[0];

      model->set_name(name_input.get_value());
      model->set_user(user_input.get_value());
      model->set_build_id(build_input.value<ODB::id_type>());
      model->set_machine_id(machine_input.value<ODB::id_type>());
      model->set_variable_list(variables_input.get_value());
      model->set_auto_deploy(auto_deploy_input.get_checked());
    }

    void trigger_binding_updates()
    {
      ModelForm::trigger_binding_updates();
      if (model)
      {
        auto_deploy_input.checked(model->get_auto_deploy());
        machine_input.set_value(model->get_machine_id());
        build_input.set_value(model->get_build_id());
      }
    }
  };
}

#endif
