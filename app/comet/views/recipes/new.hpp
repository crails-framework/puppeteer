#ifndef  VIEW_RECIPE_NEW_HPP
# define VIEW_RECIPE_NEW_HPP

# include "../utility/model_form.hpp"
# include "../utility/select.hpp"
# include "app/comet/models/recipe.hpp"
# include "app/comet/collections/credentials.hpp"
# include "html/views/recipes/new.hpp"

namespace Views
{
  class RecipeNew : public ModelForm<Front::Recipe, HtmlTemplate::RecipeNew>
  {
  public:
    RecipeNew();
    virtual ~RecipeNew() {}

    void initialize_breadcrumbs();

    std::string   get_title() const { return has_model() ? model->get_name() : "New recipe"; }
    std::string   get_recipe_name() const { return has_model() ? model->get_name() : ""; }
    std::string   get_recipe_git_url() const { return has_model() ? model->get_git_url() : ""; }
    std::string   get_recipe_git_branch() const { return has_model() ? model->get_git_branch() : ""; }
    unsigned long get_recipe_credentials_id() const { return has_model() ? model->get_credential_id() : ODB_NULL_ID; }

    void update_model_attributes();
  };
}

#endif
