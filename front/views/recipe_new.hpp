#ifndef  VIEW_RECIPE_NEW_HPP
# define VIEW_RECIPE_NEW_HPP

# include <boost/lexical_cast.hpp>
# include "utility/model_form.hpp"
# include "utility/select.hpp"
# include "../app/recipes.hpp"
# include "../app/credentials.hpp"

# include "lib/cheerp-html/views/recipe_new.hpp"

namespace Views
{
  class RecipeNew : public ModelForm<Puppeteer::Recipe, HtmlTemplate::RecipeNew>
  {
  public:
    std::string   get_title() const { return model ? model->get_name() : "New recipe"; }
    std::string   get_recipe_name() const { return model ? model->get_name() : ""; }
    std::string   get_recipe_git_url() const { return model ? model->get_git_url() : ""; }
    std::string   get_recipe_git_branch() const { return model ? model->get_git_branch() : ""; }
    unsigned long get_recipe_credentials_id() const { return model ? model->get_credential_id() : ODB_NULL_ID; }

    void update_model_attributes()
    {
      auto name_input    = find("[name=\"recipe_name\"]")[0];
      auto git_input     = find("[name=\"recipe_git_url\"")[0];
      auto branch_input  = find("[name=\"recipe_git_branch\"")[0];

      model->set_name(name_input.get_value());
      model->set_git_url(git_input.get_value());
      model->set_git_branch(branch_input.get_value());
      model->set_credential_id(credential_input.value<ODB::id_type>());
    }
  };
}

#endif
