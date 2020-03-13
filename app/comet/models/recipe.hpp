#ifndef  FRONT_RECIPE_HPP
# define FRONT_RECIPE_HPP

# include "application.hpp"
# include "app/models/recipe.hpp"

namespace Front
{
  class Recipe : public ::Recipe
  {
  public:
    model_class("recipes")
    model_default_constructors(Recipe)
  };
}

#endif

