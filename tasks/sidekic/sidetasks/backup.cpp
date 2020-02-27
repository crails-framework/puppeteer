#include "app/models/backup.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sync/task.hpp>

using namespace std;
using namespace Crails;

void backup_sidetask(Params& params)
{
  ODB::Connection database;
  shared_ptr<Backup> backup;

  if (database.find_one(backup, params["backup_id"].as<ODB::id_type>()))
  {
    string build_name = params["build_name"].as<string>();

    try
    {
      Sync::Task task(5);

      backup->perform(Backup::BackupAction, build_name, task);
      task.increment();
    }
    catch (const std::exception& e)
    {
      logger << Logger::Error << "Failed to complete backup " << backup->get_id() << ":\n\t";
      logger << e.what() << Logger::endl;
    }
    catch (...)
    {
      logger << Logger::Error << "Failed to complete backup " << backup->get_id() << Logger::endl;
    }
  }
  else
    logger << Logger::Error << "Did not find backup " << params["backup_id"].as<ODB::id_type>() << Logger::endl;
}
