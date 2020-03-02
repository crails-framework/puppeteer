#include "breadcrumbs.hpp"
#include "app/machines.hpp"
#include "app/instances.hpp"
#include "app/builds.hpp"
#include "app/recipes.hpp"
#include "app/credentials.hpp"
#include <iostream>
#define MAX_CRUMBS_DEPTH 5

using namespace std;

Breadcrumbs* Breadcrumbs::instance = nullptr;

Breadcrumbs::Breadcrumbs()
{
  crumbs.reserve(MAX_CRUMBS_DEPTH);
  set_crumb(0, "Dashboard", "#/");
  instance = this;
}

Breadcrumbs::~Breadcrumbs()
{
  instance = nullptr;
}

void Breadcrumbs::reset()
{
  if (instance)
    instance->crumbs.resize(1);
}

void Breadcrumbs::done()
{
  if (instance)
    instance->signaler.trigger("crumbs-changed");
  else
    std::cerr << "Calling Breadcrumbs::done, but Breadcrumbs haven't been initailized" << std::endl;
}

void Breadcrumbs::set_machine(ODB::id_type id)
{
  instance->set_model<Puppeteer::Machine>(id, "Machine", "Machines");
}

void Breadcrumbs::set_instance(ODB::id_type id)
{
  instance->set_model<Puppeteer::Instance>(id, "Instances", "Instance");
}

void Breadcrumbs::set_build(ODB::id_type id)
{
  instance->set_model<Puppeteer::Build>(id, "Builds", "Build");
}

void Breadcrumbs::set_recipe(ODB::id_type id)
{
  instance->set_model<Puppeteer::Recipe>(id, "Recipes", "Recipe");
}

void Breadcrumbs::set_credential(ODB::id_type id)
{
  instance->set_model<Puppeteer::Credential>(id, "Credentials", "Credential");
}

void Breadcrumbs::set_index_crumbs(const string& label, const string& path)
{
  reset();
  if (instance)
    instance->set_crumb(1, label, path);
  done();
}

void Breadcrumbs::add_crumb(const string& label, const string& path)
{
  if (instance)
  {
    unsigned short index = instance->crumbs.size();

    instance->set_crumb(index, label, path);
  }
}

void Breadcrumbs::set_crumb(unsigned short index, const string& label, const string& path)
{
  if (crumbs.size() < index)
  {
    if (crumbs[index].path != path)
      crumbs[index] = Crumb(label, path);
  }
  else
  {
    crumbs.resize(index);
    crumbs[index] = Crumb(label, path);
  }
}
