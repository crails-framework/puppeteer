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
#include "views/variable_set_new.hpp"

#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace Crails::Front;

Puppeteer::Router* Puppeteer::Router::instance = nullptr;

template<typename INDEX, typename SHOW, typename EDIT>
void make_routes_for(Puppeteer::Router* router, const std::string& path)
{
  std::cout << "make_routes_for debug#1" << std::endl;
  router->match(path, [](const Params&)
  {
    auto view = std::make_shared<INDEX>();

    MainView::instance->attach(view);
    view->activate();
  });

  std::cout << "make_routes_for debug#2" << std::endl;
  router->match(path + "/new", [](const Params&)
  {
    auto view = std::make_shared<EDIT>();

    MainView::instance->attach(view);
    view->activate();
  });

  std::cout << "make_routes_for debug#3" << std::endl;
  router->match(path + "/:resource_id", [](const Params& params)
  {
    auto view = std::make_shared<SHOW>();
    auto id   = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(view);
    view->activate(id);
  });

  std::cout << "make_routes_for debug#4" << std::endl;
  router->match(path + "/:resource_id/edit", [](const Params& params)
  {
    auto view = std::make_shared<EDIT>();
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(view);
    view->activate(id);
  });

  std::cout << "make_routes_for debug#5" << std::endl;
  router->match(path + "/:resource_id/destroy", [](const Params& params)
  {
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    __asm__("throw 'not implemented yet'");
  });
}

std::shared_ptr<Views::VariableSetForm> tintin;

void Puppeteer::Router::initialize()
{
  std::cout << "Router initialize begin" << std::endl;
  instance = this;

  std::cout << "Debug #1" << std::endl;
  make_routes_for<Views::Machines,    Views::Machine,    Views::MachineNew>   (instance, "/machines");
  std::cout << "Debug #2" << std::endl;
  make_routes_for<Views::Builds,      Views::Build,      Views::BuildNew>     (instance, "/builds");
  std::cout << "Debug #3" << std::endl;
  make_routes_for<Views::Instances,   Views::Instance,   Views::InstanceNew>  (instance, "/instances");
  std::cout << "Debug #4" << std::endl;
  make_routes_for<Views::Recipes,     Views::Recipe,     Views::RecipeNew>    (instance, "/recipes");
  std::cout << "Debug #5" << std::endl;
  make_routes_for<Views::Credentials, Views::Credential, Views::CredentialNew>(instance, "/credentials");
  std::cout << "Debug #6" << std::endl;

  match("/variables", [](const Params& params)
  {
    auto view = std::make_shared<Views::VariableSetForm>();

    tintin = view;
    MainView::instance->attach(view);
    view->activate();
  });
  std::cout << "Debug #7" << std::endl;

  match("/", [](const Params& params)
  {
  });
  std::cout << "Debug #8" << std::endl;

  std::cout << "Router initialize end" << std::endl;
  Crails::Front::Router::initialize();
  std::cout << "Router initialize base end" << std::endl;
}
