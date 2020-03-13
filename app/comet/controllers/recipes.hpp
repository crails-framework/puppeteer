#ifndef  RECIPES_CONTROLLER_HPP
# define RECIPES_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/comet/collections/recipes.hpp"
# include "app/comet/views/recipes/index.hpp"
# include "app/comet/views/recipes/new.hpp"
# include "app/comet/views/recipes/destroy.hpp"
# include "app/comet/views/recipes/show.hpp"

struct RecipeControllerDesc
{
  static const std::string resource_path;
  typedef Front::Recipes       Collection;
  typedef Views::RecipeIndex   IndexView;
  typedef Views::RecipeShow    ShowView;
  typedef Views::RecipeNew     CreateView;
  typedef Views::RecipeNew     EditView;
  typedef Views::RecipeDestroy DestroyView;
};

typedef CrudController<RecipeControllerDesc> RecipesController;

#endif

