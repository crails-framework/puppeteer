#include "app/models/instance.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sync/task.hpp>

using namespace std;
using namespace Crails;

void deploy_configuration(Params& params)
{
  Db::Connection database;
  std::shared_ptr<Instance> instance;

  try
  {
    Sync::Task sync_task(params["sidekic"]["task_uid"], 10);

    database.find_one(instance, podb::query<Instance>::id == params["id"].as<Db::id_type>());
    instance->configure(sync_task);
    database.save(*instance);
    database.commit();
    sync_task.increment();
  }
  catch (...)
  {
    if (instance)
    {
      instance->set_state(Instance::Dirty);
      database.save(*instance);
      database.commit();
    }
    throw ;
  }
}

void uninstall_configuration(Params& params)
{
  Db::Connection database;
  std::shared_ptr<Instance> instance;

  try
  {
    Sync::Task sync_task(params["sidekic"]["task_uid"], 10);

    database.find_one(instance, podb::query<Instance>::id == params["id"].as<Db::id_type>());
    instance->uninstall(sync_task);
    database.save(*instance);
    database.commit();
    sync_task.increment();
  }
  catch (...)
  {
    if (instance)
    {
      instance->set_state(Instance::Dirty);
      database.save(*instance);
      database.commit();
    }
    throw ;
  }
}
