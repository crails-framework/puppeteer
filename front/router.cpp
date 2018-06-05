#include "router.hpp"
#include "views/utility/main_view.hpp"
#include "views/machines.hpp"
#include "views/machine_new.hpp"
#include "views/machine.hpp"
#include "views/builds.hpp"
#include "views/build_new.hpp"
#include "views/build.hpp"
#include "views/instances.hpp"
#include "views/instance_new.hpp"
#include "views/instance.hpp"
#include "views/recipes.hpp"
#include "views/recipe_new.hpp"
#include "views/recipe.hpp"
#include "views/credentials.hpp"
#include "views/credential.hpp"
#include "views/credential_new.hpp"

#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace Crails::Front;

Puppeteer::Router* Puppeteer::Router::instance = nullptr;

template<typename INDEX, typename SHOW, typename EDIT>
void make_routes_for(Puppeteer::Router& router, const std::string& path)
{
  router.match(path, [](const Params&)
  {
    auto* view = new INDEX;

    MainView::instance->attach(*view);
  });

  router.match(path + "/new", [](const Params&)
  {
    auto* view = new EDIT;

    MainView::instance->attach(*view);
    view->activate();
  });

  router.match(path + "/:resource_id", [](const Params& params)
  {
    auto* view = new SHOW;
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(*view);
    view->activate(id);
  });

  router.match(path + "/:resource_id/edit", [](const Params& params)
  {
    auto* view = new EDIT;
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(*view);
    view->activate(id);
  });
}

void Puppeteer::Router::initialize()
{
  instance = this;

  make_routes_for<Views::Machines,    Views::Machine,    Views::MachineNew>   (*this, "/machines");
  make_routes_for<Views::Builds,      Views::Build,      Views::BuildNew>     (*this, "/builds");
  make_routes_for<Views::Instances,   Views::Instance,   Views::InstanceNew>  (*this, "/instances");
  make_routes_for<Views::Recipes,     Views::Recipe,     Views::RecipeNew>    (*this, "/recipes");
  make_routes_for<Views::Credentials, Views::Credential, Views::CredentialNew>(*this, "/credentials");

  Crails::Front::Router::initialize();
}
