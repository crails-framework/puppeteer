#ifndef  RECIPE_CONTROLLER_HPP
# define RECIPE_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/recipe.hpp"

class RecipeController : public Crud::ArchiveController<Recipe>
{
  typedef Crud::ArchiveController<Recipe> Super;
  std::string get_view_path() const { return "recipe_"; }
public:
  RecipeController(Crails::Params&);

  void fetch();
};

#endif

