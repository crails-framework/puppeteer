#include <boost/lexical_cast.hpp>
#include "router.hpp"
#include "views/utility/main_view.hpp"
#include "views/backup.hpp"
#include "views/backup_new.hpp"

using namespace std;
using namespace Crails::Front;

static void backup_show(const Params& params)
{
  auto id = boost::lexical_cast<unsigned long>(params.at("id"));

  Puppeteer::Backup::fetch_for_instance(id, [id](shared_ptr<Puppeteer::Backup> model)
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
}

static void backup_edit(const Params& params)
{
  auto id = boost::lexical_cast<unsigned long>(params.at("id"));

  Puppeteer::Backup::fetch_for_instance(id, [](shared_ptr<Puppeteer::Backup> model)
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
}

void Puppeteer::Router::initialize_backup_routes()
{
  match("/instances/:id/backup",      &backup_show);
  match("/instances/:id/backup/edit", &backup_edit);
}
