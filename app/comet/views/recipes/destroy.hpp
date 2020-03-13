#ifndef  DELETE_RECIPE_HPP
# define DELETE_RECIPE_HPP

# include "html/views/recipes/destroy.hpp"
# include "app/comet/models/recipe.hpp"
# include "../utility/delete_view.hpp"

namespace Views
{
  class RecipeDestroy : public DeleteView<HtmlTemplate::RecipeDestroy, Front::Recipe>
  {
  };
}

#endif
