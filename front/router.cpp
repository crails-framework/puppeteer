#include "router.hpp"
#include "views/utility/main_view.hpp"
#include "views/machines.hpp"
#include "views/machine_new.hpp"
#include "views/machine.hpp"
#include "views/builds.hpp"
#include "views/build_new.hpp"
#include "views/instances.hpp"
#include "views/instance_new.hpp"

#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace Crails::Front;

Puppeteer::Router* Puppeteer::Router::instance = nullptr;

void Puppeteer::Router::initialize()
{
  instance = this;

  match("/machines", [](const Params&)
  {
    auto* view = new Views::Machines;

    MainView::instance->attach(*view);
  });

  match("/machines/new", [](const Params&)
  {
    auto* view = new Views::MachineNew;

    MainView::instance->attach(*view);
    view->activate();
  });

  match("/machines/:machine_id", [](const Params& params)
  {
    auto id = boost::lexical_cast<unsigned long>(params.at("machine_id"));
    auto* view = new Views::Machine;

    MainView::instance->attach(*view);
    view->activate(id);
  });

  match("/builds", [](const Params&)
  {
    auto* view = new Views::Builds;

    MainView::instance->attach(*view);
  });

  match("/builds/new", [](const Params&)
  {
    auto* view = new Views::BuildNew;

    MainView::instance->attach(*view);
    view->activate();
  });

  match("/instances", [](const Params&)
  {
    auto* view = new Views::Instances;

    MainView::instance->attach(*view);
  });

  match("/instances/new", [](const Params&)
  {
    auto* view = new Views::InstanceNew;

    MainView::instance->attach(*view);
    view->activate();
  });
  Crails::Front::Router::initialize();
}
