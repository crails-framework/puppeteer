#ifndef  FRONT_RECIPES_HPP
# define FRONT_RECIPES_HPP

# include "data/recipe.hpp"
# include "model.hpp"
# include <crails/front/mvc/collection.hpp>

namespace Puppeteer
{
  class Recipe : public RecipeData
  {
  public:
    model_class("recipes")
    model_default_constructors(Recipe)
  };

  class Recipes : public Crails::Front::Collection<Recipe>
  {
  public:
    std::string get_url() const
    {
      return '/' + Recipe::get_scope();
    }
  };
}

#endif
