#include "app/models/instance.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/params.hpp>
#include <crails/sidekic.hpp>
#include <crails/logger.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace Crails;

int main(int argc, char** argv)
{
  DataTree task_params;

  if (argc == 2)
  {
    ODB::Connection database;
    ODB::id_type build_id = boost::lexical_cast<ODB::id_type>(argv[1]);
    shared_ptr<Build> build;

    if (database.find_one(build, build_id))
    {
      unsigned int history_size = build->get_history_size();

      task_params["build_id"] = build_id;
      Sidekic::async_task("auto_deploy", task_params.as_data());
      logger << Logger::Info << "Scheduled auto deploy for build " << build->get_name() << Logger::endl;
      if (history_size > 0 && build->get_available_builds().size() > history_size)
      {
        logger << Logger::Info << "Maximum build history size reached: clearing surplus builds..." << Logger::endl;
        build->clear_build_history();
        logger << Logger::Info << "Cleared surplus builds." << Logger::endl;
      }
    }
    else
    {
      logger << Logger::Error << "Build " << build_id << " not found" << Logger::endl;
      return -2;
    }
  }
  else
  {
    logger << Logger::Info << "usage: " << argv[0] << " [build_id]" << Logger::endl;
    return -1;
  }
  return 0;
}
