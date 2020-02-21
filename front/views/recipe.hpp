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
    void on_pull_clicked()
    {
      if (model)
      {
        Crails::Front::Ajax::query("POST", model->get_url() + "/fetch").callbacks({
          std::bind(&Recipe::on_fetched,      this, std::placeholders::_1),
          std::bind(&Recipe::on_fetch_failed, this, std::placeholders::_1)
        })();
      }
    }

    void on_fetched(const Crails::Front::Ajax&)
    {
      std::cout << "recipe fetched" << std::endl;
      model->fetch().then([this]() { signaler.trigger("model-changed"); });
    }

    void on_fetch_failed(const Crails::Front::Ajax&)
    {
      std::cout << "recipe fetch failed" << std::endl;
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
