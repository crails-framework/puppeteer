#include "router.hpp"
#include "views/utility/main_view.hpp"
#include "views/machines.hpp"
#include "views/machine_new.hpp"
#include "views/machine.hpp"
#include "views/delete/machine.hpp"
#include "views/builds.hpp"
#include "views/build_new.hpp"
#include "views/build.hpp"
#include "views/delete/build.hpp"
#include "views/instances.hpp"
#include "views/instance_new.hpp"
#include "views/instance.hpp"
#include "views/delete/instance.hpp"
#include "views/recipes.hpp"
#include "views/recipe_new.hpp"
#include "views/recipe.hpp"
#include "views/delete/recipe.hpp"
#include "views/credentials.hpp"
#include "views/credential.hpp"
#include "views/credential_new.hpp"
#include "views/delete/credential.hpp"
#include "views/backup.hpp"
#include "views/backup_new.hpp"
#include "views/variable_set_new.hpp"
#include "resources/modal.hpp"

#include <crails/front/globals.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using namespace Crails::Front;

Puppeteer::Router* Puppeteer::Router::instance = nullptr;

template<typename INDEX, typename SHOW, typename EDIT, typename DELETE>
void make_routes_for(Puppeteer::Router* router, const std::string& path)
{
  router->match(path, [](const Params&)
  {
    auto view = std::make_shared<INDEX>();

    MainView::instance->attach(view);
    view->activate();
  });

  router->match(path + "/new", [](const Params&)
  {
    auto view = std::make_shared<EDIT>();

    MainView::instance->attach(view);
    view->activate();
  });

  router->match(path + "/:resource_id", [](const Params& params)
  {
    auto view = std::make_shared<SHOW>();
    auto id   = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(view);
    view->activate(id);
  });

  router->match(path + "/:resource_id/edit", [](const Params& params)
  {
    auto view = std::make_shared<EDIT>();
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(view);
    view->activate(id);
  });

  router->match(path + "/:resource_id/destroy", [router, path](const Params& params)
  {
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));
    auto modal = Modal<DELETE>::make("Removing");

    modal->el.activate(id);
    modal->open().then([router, path, modal, id]()
    {
      if (modal->ok())
      {
        modal->el.destroy([router, path](bool success)
        {
          if (success)
            router->navigate(path, true);
          else
            Crails::Front::window->alert("resource removal failed");
        });
      }
      else
        router->navigate(path + '/' + boost::lexical_cast<std::string>(id), false);
    });
  });
}

std::shared_ptr<Views::VariableSetForm> tintin;

static void workaround_router_unexplained_crash(Puppeteer::Router* router)
{
  // Weirdly, if we call Crails::RouterBase::match before this code, it will crash.
  // This issue doesn't happen in all the builds of the application.
  // For now, it's only been seen on ubuntu:19.04 docker instances.
  Crails::Front::Router::Item item;

  router->routes.push_back(item);
  router->routes.empty();
}

void Puppeteer::Router::initialize()
{
  instance = this;

  workaround_router_unexplained_crash(instance);

  make_routes_for<Views::Machines,    Views::Machine,    Views::MachineNew,    Views::DeleteMachine>   (instance, "/machines");
  make_routes_for<Views::Builds,      Views::Build,      Views::BuildNew,      Views::DeleteBuild>     (instance, "/builds");
  make_routes_for<Views::Instances,   Views::Instance,   Views::InstanceNew,   Views::DeleteInstance>  (instance, "/instances");
  make_routes_for<Views::Recipes,     Views::Recipe,     Views::RecipeNew,     Views::DeleteRecipe>    (instance, "/recipes");
  make_routes_for<Views::Credentials, Views::Credential, Views::CredentialNew, Views::DeleteCredential>(instance, "/credentials");

  match("/instances/:id/backup", [](const Params& params)
  {
    auto id = boost::lexical_cast<unsigned long>(params.at("id"));

    Puppeteer::Backup::fetch_for_instance(id, [id](shared_ptr<Backup> model)
    {
      if (model)
      {
        auto view = std::make_shared<Views::Backup>();

	MainView::instance->attach(view);
	view->activate(model);
      }
      else
      {
        auto view = std::make_shared<Views::BackupNew>();

	view->set_instance_id(id);
	MainView::instance->attach(view);
	view->activate();
      }
    });
  });

  match("/instances/:id/backup/edit", [](const Params& params)
  {
    auto id = boost::lexical_cast<unsigned long>(params.at("id"));

    Puppeteer::Backup::fetch_for_instance(id, [](shared_ptr<Backup> model)
    {
      if (model)
      {
        auto view = std::make_shared<Views::BackupNew>();

        MainView::instance->attach(view);
        view->activate(model);
      }
      else
      {
        std::cout << "ERROR: no backup settings found for this instance" << std::endl;
      }
    });
  });

  match("/variables", [](const Params& params)
  {
    auto view = std::make_shared<Views::VariableSetForm>();

    tintin = view;
    MainView::instance->attach(view);
    view->activate();
  });

  match("/", [](const Params& params)
  {
  });

  Crails::Front::Router::initialize();
}
