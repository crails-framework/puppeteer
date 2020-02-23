#include "app/models/instance.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sync/task.hpp>

using namespace std;
using namespace Crails;

void run_instance_script(Params& params)
{
  ODB::Connection database;
  std::shared_ptr<Instance> instance;

  try
  {
    Sync::Task sync_task(params["sidekic"]["task_uid"], 10);

    if (database.find_one(instance, params["id"].as<ODB::id_type>()))
    {
      auto recipe = instance->get_build()->get_recipe();

      recipe->exec_script(params["script_name"].as<std::string>(), *instance, sync_task);
      instance->set_running_task("");
      database.save(*instance);
      database.commit();
      sync_task.increment();
    }
  }
  catch (...)
  {
    if (instance)
    {
      instance->set_running_task("");
      database.save(*instance);
      database.commit();
    }
    throw ;
  }
}
