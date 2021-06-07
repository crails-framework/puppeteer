#ifndef  VIEW_INSTANCE_NEW_HPP
# define VIEW_INSTANCE_NEW_HPP

# include "../utility/model_form.hpp"
# include "../utility/select.hpp"
# include "../utility/variable_list_editor.hpp"
# include "app/comet/models/instance.hpp"
# include "html/views/instances/new.hpp"

namespace Views
{
  class InstanceNew : public ModelForm<Front::Instance, HtmlTemplate::InstanceNew>
  {
  public:
    InstanceNew();
    virtual ~InstanceNew() {}

    void initialize_breadcrumbs();

    std::string   get_title() const { return has_model() ? model->get_name() : "New instance"; }
    std::string   get_instance_name() const       { return has_model() ? model->get_name() : ""; }
    std::string   get_instance_user() const       { return has_model() ? model->get_user() : ""; }
    unsigned long get_instance_build_id() const   { return has_model() ? model->get_build_id() : ODB_NULL_ID; }
    unsigned long get_instance_machine_id() const { return has_model() ? model->get_machine_id() : ODB_NULL_ID; }
    unsigned long get_instance_recipe_id() const  { return has_model() ? model->get_recipe_id() : ODB_NULL_ID; }
    std::string   get_instance_variables() const  { return has_model() ? model->get_variables() : ""; }

    void update_model_attributes();
    void update_recipe_input_visibility();
    void trigger_binding_updates();
  };
}

#endif
