#include "new.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"

using namespace std;

void Views::MachineNew::initialize_breadcrumbs()
{
  auto id = has_model() ? model->get_id() : ODB_NULL_ID;

  Breadcrumbs::reset();
  Breadcrumbs::set_machine(id);
  if (id == ODB_NULL_ID)
    Breadcrumbs::add_crumb("New", "#/machines/new");
  else
    Breadcrumbs::add_crumb("Edit", model->get_path() + "/edit");
  Breadcrumbs::done();
}

void Views::MachineNew::update_model_attributes()
{
  auto name_input = find("[name=\"machine_name\"]")[0];
  auto ip_input   = find("[name=\"machine_ip\"]")[0];

  model->set_name(name_input.get_value());
  model->set_ip(ip_input.get_value());
}
