#include <crails/logger.hpp>
#include <crails/odb/connection.hpp>
#include <crails/sidekic.hpp>
#include "lib/odb/application-odb.hxx"
#include "app/jenkins/jenkins.hpp"
#include "app/models/build.hpp"
#include "app/models/instance.hpp"

using namespace Crails;
using namespace std;

int main(int argc, char** argv)
{
  ODB::Connection     database;
  odb::result<Build>  builds;
  Jenkins jenkins;
  list<pair<unsigned int, Instance> > instances_to_update;

  logger << Logger::Info << "# Synchronizing builds with jenkins" << Logger::endl;
  database.find(builds);
  for (auto& build : builds)
  {
    DataTree data = jenkins.get_project_data(build.get_name());

    if (build.update_last_build(data))
    {
      odb::result<Instance> instances;

      database.find<Instance>(instances, odb::query<Instance>::build_id    == build.get_id()
                                      && odb::query<Instance>::auto_deploy == true);
      for (const auto& instance : instances)
        instances_to_update.push_back(make_pair(build.get_last_build(), instance));
    }
  }

  for (auto& update_data : instances_to_update)
  {
    unsigned int build_id = update_data.first;
    Instance&    instance = update_data.second;
    DataTree     task_params;
    string       task_uid;

    logger << Logger::Info << "# Scheduling build for " << instance.get_name() << Logger::endl;
    task_params["id"]       = instance.get_id();
    task_params["build_id"] = build_id;
    task_uid = Sidekic::async_task("deploy_build", task_params.as_data());
    instance.set_running_task("deploy_build:" + task_uid);
    database.save(instance);
    database.commit();
  }

  return 0;
}
