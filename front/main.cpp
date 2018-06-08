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

Sync::Faye*  faye;
Sync::Tasks* sync_tasks;

void webMain()
{
  std::cout << "cheerp client started ma gueule" << std::endl;

  auto* router = new Puppeteer::Router;
  MainView* main_view = new MainView;

  MainView::instance = new MainView;

  TemplateView my_view("tpl1");

  main_view->attach(my_view);

  //my_view.emplace(Crails::Front::body);

  router->initialize();
  std::string klass = Crails::Front::body.attr("class");
  std::cout << "body has class " << klass << std::endl;
  //body.text("coucou, tu veux voir ma bite ?");
 
  faye       = new Sync::Faye;
  sync_tasks = new Sync::Tasks(*faye);

  faye->subscribe("/sync", [](Crails::Front::Object response)
  {
    std::string text = response["text"];

    std::cout << "Native faye handler says: " << text << std::endl;
  });
}
