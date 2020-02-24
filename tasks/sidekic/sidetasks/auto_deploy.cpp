#include "app/models/instance.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sync/task.hpp>

using namespace std;
using namespace Crails;

static list<Instance> fetch_instances(ODB::Connection& database, ODB::id_type build_id)
{
  odb::result<Instance> results;
  list<Instance> instances;

  database.find<Instance>(results, odb::query<Instance>::build_id == build_id && odb::query<Instance>::auto_deploy == true);
  for (const auto& result : results)
    instances.push_back(result);
  return instances;
}

static std::string get_last_build_id_for(Build& build)
{
  stringstream stream;
  build.update_last_build();

  stream << build.get_last_build();
  return stream.str();
}

void auto_deploy(Params& params)
{
  ODB::Connection   database;
  list<Instance>    instances = fetch_instances(database, params["build_id"]);
  shared_ptr<Build> build;

  if (database.find_one(build, params["build_id"].as<ODB::id_type>()))
  {
    string last_build = get_last_build_id_for(*build);

    for (auto instance : instances)
    {
      if (instance.get_state() == Instance::Uninstalled)
        continue ;
      try
      {
        string     task_uid = generate_random_string("abcdefghijklmnopqrstuvwxyz", 10);
        Sync::Task sync_task(task_uid, 10);

        instance.set_running_task("deploy_build:" + task_uid);
        database.save(instance);
        database.commit();

        instance.deploy(sync_task, last_build);
        instance.stop(sync_task);
        instance.start(sync_task);

        instance.set_state(Instance::Deployed);
        instance.set_running_task("");
        database.save(instance);
        database.commit();
      }
      catch (...)
      {
        instance.set_state(Instance::Dirty);
        instance.set_running_task("");
        database.save(instance);
        database.commit();
      }
    }
  }
  else
    logger << Logger::Error << "Did not found build " << params["build_id"].as<ODB::id_type>() << Logger::endl;
}
