#ifndef  VIEW_RECIPES_HPP
# define VIEW_RECIPES_HPP

# include "../utility/index_view.hpp"
# include "app/comet/collections/recipes.hpp"

namespace Views
{
  class RecipeIndex : public IndexView<Front::Recipes>
  {
  public:
    RecipeIndex(std::shared_ptr<Front::Recipes> c) : IndexView(c)
    {
    }

    virtual ~RecipeIndex() {}

    std::string              get_title() const          { return "Recipes"; }
    std::string              get_new_model_path() const { return "#/recipes/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name", "git url", "git branch"}; }

    Elements make_columns_for(std::shared_ptr<Front::Recipe> model) const
    {
      return {
        Comet::Element("td").inner({
          Comet::Element("a", {{"href",model->get_path()}}).text(model->get_name())
        }),
        Comet::Element("td").text(model->get_git_url()),
        Comet::Element("td").text(model->get_git_branch())
      };
    }
  };
}

#endif
