#ifndef  VIEW_RECIPES_HPP
# define VIEW_RECIPES_HPP

# include "utility/table_view.hpp"
# include "utility/theme.hpp"
# include "../app/recipes.hpp"

namespace Views
{
  class Recipes : public TableView<Puppeteer::Recipes>
  {
  public:
    Recipes()
    {
      title = "Recipes";
    }

    std::string get_new_model_path() const { return "#/recipes/new"; }

    std::vector<std::string> get_columns() const { return {"name", "git_url", "git_branch"}; }

    std::vector<El> make_columns_for(std::shared_ptr<Puppeteer::Recipe> model) const
    {
      return {
        El("td").inner({
          El("a", {{"href",model->get_path()}}).text(model->get_name())
        }),
        El("td").text(model->get_git_url()),
        El("td").text(model->get_git_branch())
      };
    }

  private:
  };
}

#endif
