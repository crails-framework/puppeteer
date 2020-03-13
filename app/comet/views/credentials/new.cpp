#include "new.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"

using namespace std;

void Views::CredentialNew::initialize_breadcrumbs()
{
  auto id = model ? model->get_id() : ODB_NULL_ID;

  Breadcrumbs::reset();
  Breadcrumbs::set_credential(id);
  if (id == ODB_NULL_ID)
    Breadcrumbs::add_crumb("New", "#/credentials/new");
  else
    Breadcrumbs::add_crumb("Edit", model->get_path() + "/edit");
  Breadcrumbs::done();
}

void Views::CredentialNew::update_model_attributes()
{
  auto name_input  = find("[name=\"credentials_name\"]")[0];
  auto user_input  = find("[name=\"credentials_user\"")[0];
  auto pswd_input  = find("[name=\"credentials_pswd\"")[0];

  model->set_name(name_input.get_value());
  model->set_username(user_input.get_value());
  model->set_password(pswd_input.get_value());
}
