#ifndef  VIEW_RECIPE_HPP
# define VIEW_RECIPE_HPP

# include "utility/model_view.hpp"
# include "../app/recipes.hpp"

namespace Views
{
  class Recipe : public ModelView<Puppeteer::Recipe>
  {
  public:
    Recipe() : ModelView("Recipe")
    {
      
    }
  };
}

#endif
