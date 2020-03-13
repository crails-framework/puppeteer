#include "new.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"

using namespace std;

Views::RecipeNew::RecipeNew()
{
  credential_input.with_empty_option(true);
}

void Views::RecipeNew::initialize_breadcrumbs()
{
  auto id = model ? model->get_id() : ODB_NULL_ID;

  Breadcrumbs::reset();
  Breadcrumbs::set_recipe(id);
  if (id == ODB_NULL_ID)
    Breadcrumbs::add_crumb("New", "#/recipes/new");
  else
    Breadcrumbs::add_crumb("Edit", model->get_path() + "/edit");
  Breadcrumbs::done();
}

void Views::RecipeNew::update_model_attributes()
{
  auto name_input    = find("[name=\"recipe_name\"]")[0];
  auto git_input     = find("[name=\"recipe_git_url\"")[0];
  auto branch_input  = find("[name=\"recipe_git_branch\"")[0];

  model->set_name(name_input.get_value());
  model->set_git_url(git_input.get_value());
  model->set_git_branch(branch_input.get_value());
  model->set_credential_id(credential_input.value<ODB::id_type>());
}
