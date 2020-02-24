#include "app/models/instance.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sync/task.hpp>

using namespace std;
using namespace Crails;

void deploy_build(Params& params)
{
  ODB::Connection database;
  std::shared_ptr<Instance> instance;

  try
  {
    Sync::Task sync_task(params["sidekic"]["task_uid"], 10);

    sync_task.set_task_count(5 + 3 + 3);
    if (database.find_one(instance, params["id"].as<ODB::id_type>()))
    {
      if (instance->get_state() != Instance::Uninstalled)
      {
        instance->deploy(sync_task, params["build_id"]);
        try
        {
          instance->stop(sync_task);
          instance->start(sync_task);
        }
        catch (...)
        {
          logger << Logger::Error << "Failed to restart instance " << instance->get_name() << " after deploy" << Logger::endl;
        }
        instance->set_running_task("");
        database.save(*instance);
        database.commit();
        sync_task.increment();
      }
      else
        logger << Logger::Error << "Cannot deploy on instance " << instance->get_name() << ": not installed" << Logger::endl;
    }
  }
  catch (...)
  {
    if (instance)
    {
      instance->set_state(Instance::Dirty);
      instance->set_running_task("");
      database.save(*instance);
      database.commit();
    }
    throw ;
  }
}
