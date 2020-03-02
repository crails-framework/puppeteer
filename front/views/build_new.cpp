#include "build_new.hpp"
#include "front/resources/elements/breadcrumbs.hpp"

Views::BuildNew::BuildNew()
{
  credentials_input.with_empty_option(true);
}

void Views::BuildNew::initialize_breadcrumbs()
{
  auto id = model ? model->get_id() : ODB_NULL_ID;

  Breadcrumbs::reset();
  Breadcrumbs::set_build(id);
  if (id == ODB_NULL_ID)
    Breadcrumbs::add_crumb("New", "#/builds/new");
  else
    Breadcrumbs::add_crumb("Edit", model->get_path() + "/edit");
  Breadcrumbs::done();
}

void Views::BuildNew::update_model_attributes()
{
  auto name_input    = find("[name=\"build_name\"]")[0];
  auto git_input     = find("[name=\"build_git\"")[0];
  auto branch_input  = find("[name=\"build_branch\"")[0];
  auto options_input = find("[name=\"build_options\"")[0];

  model->set_name(name_input.get_value());
  model->set_git(git_input.get_value());
  model->set_branch(branch_input.get_value());
  model->set_credential_id(credentials_input.value<ODB::id_type>());
  model->set_recipe_id(recipe_input.value<ODB::id_type>());
  model->set_options(options_input.get_value());
  model->set_variable_list(variables_input.get_value());
}
