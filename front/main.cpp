#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include <iostream>

#include <crails/front/object.hpp>
#include <crails/front/globals.hpp>

#include "router.hpp"
#include "views/utility/template_view.hpp"
#include "views/utility/main_view.hpp"

void onReady()
{
  std::cout << "cheerp client ready maggle" << std::endl;
}

auto onReady_callback = cheerp::Callback(onReady);

using namespace Crails::Front;

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
}
