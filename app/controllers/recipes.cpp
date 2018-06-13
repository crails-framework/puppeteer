#include "lib/odb/application-odb.hxx"
#include "recipes.hpp"

using namespace std;
using namespace Crails;

RecipeController::RecipeController(Params& params) : Super(params)
{
}

void RecipeController::fetch()
{
  require_model();
  if (model)
  {
    model->fetch_recipe();
    database.save(*model);
  }
}
