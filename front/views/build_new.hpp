#ifndef  VIEW_BUILD_NEW_HPP
# define VIEW_BUILD_NEW_HPP

# include "utility/model_form.hpp"
# include "utility/select.hpp"
# include "utility/variable_list_editor.hpp"
# include "../app/builds.hpp"
# include "../app/recipes.hpp"
# include "../app/credentials.hpp"
# include "lib/cheerp-html/views/build_new.hpp"

namespace Views
{
  class BuildNew : public ModelForm<Puppeteer::Build, HtmlTemplate::BuildNew>
  {
  public:
    std::string get_title() const { return model ? model->get_name() : "New build"; }
    std::string get_build_name() const { return model ? model->get_name() : ""; }
    std::string get_build_git_url() const { return model ? model->get_git() : ""; }
    std::string get_build_branch() const { return model ? model->get_branch() : ""; }
    std::string get_build_options() const { return model ? model->get_options() : ""; }
    unsigned long get_build_credentials_id() const { return model ? model->get_credential_id() : ODB_NULL_ID; }
    unsigned long get_build_recipe_id() const { return model ? model->get_recipe_id() : ODB_NULL_ID; }
    std::string   get_build_variables() const { return model ? model->get_variables() : ""; }

    void update_model_attributes()
    {
      auto name_input    = find("[name=\"build_name\"]")[0];
      auto git_input     = find("[name=\"build_git_url\"")[0];
      auto branch_input  = find("[name=\"build_branch\"")[0];
      auto options_input = find("[name=\"build_options\"")[0];

      model->set_name(name_input.get_value());
      model->set_git(git_input.get_value());
      model->set_branch(branch_input.get_value());
      model->set_credential_id(credentials_input.value<ODB::id_type>());
      model->set_recipe_id(recipe_input.value<ODB::id_type>());
      model->set_options(options_input.get_value());
      model->set_variable_list(variables_input.get_value());
    }
  };
}

#endif
