#include <crails/odb/connection.hpp>
#include <crails/logger.hpp>
#include <crails/params.hpp>
#include <crails/sidekic.hpp>
#include <crails/logger.hpp>
#include <boost/lexical_cast.hpp>
#include "app/models/backup.hpp"
#include "lib/odb/application-odb.hxx"
#include "app/recipe_runners/backup_runner.hpp"

using namespace std;
using namespace Crails;

int main(int argc, char** argv)
{
  DataTree task_params;

  if (argc == 3)
  {
    ODB::Connection database;
    ODB::id_type       backup_id  = boost::lexical_cast<ODB::id_type>(argv[1]);
    string             build_name = argv[2];
    shared_ptr<Backup> backup;

    if (database.find_one(backup, backup_id))
    {
      auto instance = backup->get_instance();

      if (instance && instance->get_state() > Instance::Ready)
      {
        task_params["backup_id"]  = backup_id;
        task_params["build_name"] = build_name;
        Sidekic::async_task("backup", task_params.as_data());
        logger << Logger::Info << "Successfully scheduled backup " << backup_id << Logger::endl;
      }
      else if (instance)
      {
        logger << Logger::Error << "Instance " << instance->get_name() << " is not in an appropriate state for backup (state: " << instance->get_state() << ')' << Logger::endl;
        return -3;
      }
      else
        logger << Logger::Error << "Cannot find instance " << backup->get_instance_id() << Logger::endl;
    }
    else
    {
      logger << Logger::Error << "could not find backup " << backup_id << Logger::endl;
      return -2;
    }
  }
  else
  {
    logger << Logger::Info << "usage: " << argv[0] << " [backup_id] [build_name]" << Logger::endl;
    return -1;
  }
  return 0;
}
