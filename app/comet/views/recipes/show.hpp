#ifndef  VIEW_RECIPE_HPP
# define VIEW_RECIPE_HPP

# include "../utility/model_view.hpp"
# include "app/comet/collections/recipes.hpp"
# include <comet/http.hpp>
# include "html/views/recipes/show.hpp"

namespace Views
{
  class RecipeShow : public ModelView<Front::Recipe, HtmlTemplate::RecipeShow>
  {
    const std::string null_string;
  public:
    virtual ~RecipeShow() {}

    void initialize_breadcrumbs();

    void on_pull_clicked();
    void on_fetched(const Comet::Http::Response&);
    void on_fetch_failed(const Comet::Http::Response&);

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
