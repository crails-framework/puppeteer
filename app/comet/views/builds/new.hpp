#ifndef  VIEW_BUILD_NEW_HPP
# define VIEW_BUILD_NEW_HPP

# include "../utility/model_form.hpp"
# include "../utility/select.hpp"
# include "../utility/variable_list_editor.hpp"
# include "app/comet/collections/builds.hpp"
# include "app/comet/collections/recipes.hpp"
# include "app/comet/collections/credentials.hpp"
# include "html/views/builds/new.hpp"

namespace Views
{
  class BuildNew : public ModelForm<Front::Build, HtmlTemplate::BuildNew>
  {
  public:
    BuildNew();
    virtual ~BuildNew() {}

    void initialize_breadcrumbs();

    std::string get_title() const { return has_model() ? model->get_name() : "New build"; }
    std::string get_build_name() const { return has_model() ? model->get_name() : ""; }
    std::string get_build_git_url() const { return has_model() ? model->get_git() : ""; }
    std::string get_build_branch() const { return has_model() ? model->get_branch() : ""; }
    std::string get_build_options() const { return has_model() ? model->get_options() : ""; }
    unsigned long get_build_credentials_id() const { return has_model() ? model->get_credential_id() : ODB_NULL_ID; }
    unsigned long get_build_recipe_id() const { return has_model() ? model->get_recipe_id() : ODB_NULL_ID; }
    std::string   get_build_variables() const { return has_model() ? model->get_variables() : ""; }
    unsigned int  get_build_history_size() const { return has_model() ? model->get_history_size() : 50; }

    void update_model_attributes();

    void trigger_binding_updates()
    {
      ModelForm::trigger_binding_updates();
      if (model)
      {
        recipe_input.set_value(model->get_recipe_id());
        credentials_input.set_value(model->get_credential_id());
      }
    }
  };
}

#endif
