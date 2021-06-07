#include "new.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"

using namespace std;

Views::InstanceNew::InstanceNew()
{
  build_input.with_empty_option(true);
  build_input.events->on("change", [this]() { update_recipe_input_visibility(); });
}

void Views::InstanceNew::update_recipe_input_visibility()
{
  auto build_id = build_input.value<ODB::id_type>();

  recipe_form_group.visible(build_id != ODB_NULL_ID);
}

void Views::InstanceNew::initialize_breadcrumbs()
{
  auto id = model ? model->get_id() : ODB_NULL_ID;

  Breadcrumbs::reset();
  Breadcrumbs::set_instance(id);
  if (id == ODB_NULL_ID)
    Breadcrumbs::add_crumb("New", "#/instances/new");
  else
    Breadcrumbs::add_crumb("Edit", model->get_path() + "/edit");
  Breadcrumbs::done();
  
}

void Views::InstanceNew::update_model_attributes()
{
  auto name_input = find("[name=\"instance_name\"]")[0];
  auto user_input = find("[name=\"instance_user\"]")[0];

  model->set_name(name_input.get_value());
  model->set_user(user_input.get_value());
  model->set_build_id(build_input.value<ODB::id_type>());
  model->set_recipe_id(recipe_input.value<ODB::id_type>());
  model->set_machine_id(machine_input.value<ODB::id_type>());
  model->set_variable_list(variables_input.get_value());
  model->set_auto_deploy(auto_deploy_input.get_checked());
}

void Views::InstanceNew::trigger_binding_updates()
{
  ModelForm::trigger_binding_updates();
  if (model)
  {
    auto_deploy_input.checked(model->get_auto_deploy());
    machine_input.set_value(model->get_machine_id());
    build_input.set_value(model->get_build_id());
    recipe_input.set_value(model->get_recipe_id());
  }
}

