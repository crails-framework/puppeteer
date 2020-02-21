#ifndef  VIEW_RECIPES_HPP
# define VIEW_RECIPES_HPP

# include "utility/index_view.hpp"
# include "../app/recipes.hpp"

namespace Views
{
  class Recipes : public IndexView<Puppeteer::Recipes>
  {
  public:
    std::string              get_title() const          { return "Recipes"; }
    std::string              get_new_model_path() const { return "#/recipes/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name", "git url", "git branch"}; }

    Elements make_columns_for(std::shared_ptr<Puppeteer::Recipe> model) const
    {
      return {
        Crails::Front::Element("td").inner({
          Crails::Front::Element("a", {{"href",model->get_path()}}).text(model->get_name())
        }),
        Crails::Front::Element("td").text(model->get_git_url()),
        Crails::Front::Element("td").text(model->get_git_branch())
      };
    }
  };
}

#endif
