#ifndef  DELETE_RECIPE_HPP
# define DELETE_RECIPE_HPP

# include "lib/cheerp-html/views/delete/delete_recipe.hpp"
# include "app/recipes.hpp"
# include "view.hpp"

namespace Views
{
  class DeleteRecipe : public DeleteView<HtmlTemplate::DeleteRecipe, Puppeteer::Recipe>
  {
  };
}

#endif
