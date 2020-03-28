#include "app/models/machine.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sync/task.hpp>
#include <crails/sentry.hpp>
#include <crails/getenv.hpp>
#include "app/ssh/session.hpp"

using namespace std;
using namespace Crails;

extern thread_local Sentry sentry;

void machine_upgrade(Params& params)
{
  ODB::Connection     database;
  Sync::Task          sync_task(params["sidekic"]["task_uid"], 4);
  shared_ptr<Machine> machine;

  if (database.find_one(machine, params["machine_id"].as<ODB::id_type>()))
  {
    Sync::Stream stream(sync_task);

    sync_task.increment();
    machine->open_ssh([&](Ssh::Session& ssh)
    {
      ssh.exec("apt-get -y update",  stream);
      sync_task.increment();
      ssh.exec("apt-get -y upgrade", stream);
      sync_task.increment();
    });
    for (const auto plugin : Machine::plugins)
      plugin->upgrade(*machine, stream);
    sync_task.increment();
  }
  else
    logger << Logger::Error << "Did not find machine " << params["build_id"].as<ODB::id_type>() << Logger::endl;
}
