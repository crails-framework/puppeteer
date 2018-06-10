#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include <iostream>

#include <crails/front/object.hpp>
#include <crails/front/globals.hpp>

#include "router.hpp"
#include "views/utility/template_view.hpp"
#include "views/utility/main_view.hpp"

#include "front/app/faye.hpp"
#include "front/app/sync_task.hpp"

using namespace Crails::Front;

Sync::Faye*        faye;
Sync::Tasks*       sync_tasks;
Puppeteer::Router* router;

static void initialize()
{
  router             = new Puppeteer::Router;
  MainView::instance = new MainView;
  faye               = new Sync::Faye;
  sync_tasks         = new Sync::Tasks(*faye);
  router->initialize();
}

void webMain()
{
  std::cout << "cheerp client started ma gueule" << std::endl;
  Crails::Front::document.on_ready(initialize);
}
