#ifndef  VIEW_RECIPE_HPP
# define VIEW_RECIPE_HPP

# include "utility/model_view.hpp"
# include "utility/button.hpp"
# include "../app/recipes.hpp"
# include <crails/front/ajax.hpp>
# include "lib/cheerp-html/views/recipe.hpp"

namespace Views
{
  class Recipe : public ModelView<Puppeteer::Recipe, HtmlTemplate::Recipe>
  {
    const std::string null_string;
  public:
    void initialize_breadcrumbs();

    void on_pull_clicked();
    void on_fetched(const Crails::Front::Ajax&);
    void on_fetch_failed(const Crails::Front::Ajax&);

    const std::string get_title() const
    {
      if (get_model())
        return get_model()->get_name();
      return ModelView::get_title();
    }

    const std::string get_recipe_url() const
    {
      return model ? model->get_git_url() : null_string;
    }
      
    const std::string get_recipe_branch() const
    {
      return model ? model->get_git_branch() : null_string;
    }

    const std::string get_recipe_tip() const
    {
      return model ? model->get_last_tip() : null_string;
    }
  };
}

#endif
