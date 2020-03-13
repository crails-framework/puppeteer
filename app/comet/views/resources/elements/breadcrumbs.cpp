#include "breadcrumbs.hpp"
#include "app/comet/models/machine.hpp"
#include "app/comet/models/instance.hpp"
#include "app/comet/models/build.hpp"
#include "app/comet/models/recipe.hpp"
#include "app/comet/models/credential.hpp"
#include <iostream>
#define MAX_CRUMBS_DEPTH 5

using namespace std;

Breadcrumbs* Breadcrumbs::instance = nullptr;

Breadcrumbs::Breadcrumbs()
{
  //crumbs.reserve(MAX_CRUMBS_DEPTH);
  crumbs.push_back(Crumb("Dashboard", "#/"));
  instance = this;
  trigger_binding_updates();
}

Breadcrumbs::~Breadcrumbs()
{
  instance = nullptr;
}

void Breadcrumbs::reset()
{
  if (instance)
  {
    instance->crumbs.clear();
    instance->crumbs.push_back(Crumb("Dashboard", "#/"));
  }
}

void Breadcrumbs::done()
{
  if (instance)
    instance->trigger_binding_updates();
  else
    std::cerr << "Calling Breadcrumbs::done, but Breadcrumbs haven't been initailized" << std::endl;
}

void Breadcrumbs::set_machine(ODB::id_type id)
{
  instance->set_model<Front::Machine>(id, "Machine", "Machines");
}

void Breadcrumbs::set_instance(ODB::id_type id)
{
  instance->set_model<Front::Instance>(id, "Instance", "Instances");
}

void Breadcrumbs::set_build(ODB::id_type id)
{
  instance->set_model<Front::Build>(id, "Build", "Builds");
}

void Breadcrumbs::set_recipe(ODB::id_type id)
{
  instance->set_model<Front::Recipe>(id, "Recipe", "Recipes");
}

void Breadcrumbs::set_credential(ODB::id_type id)
{
  instance->set_model<Front::Credential>(id, "Credential", "Credentials");
}

void Breadcrumbs::set_index_crumbs(const string& label, const string& path)
{
  reset();
  if (instance)
    instance->crumbs.push_back(Crumb(label, path));
  done();
}

void Breadcrumbs::add_crumb(const string& label, const string& path)
{
  if (instance)
    instance->crumbs.push_back(Crumb(label, path));
}
