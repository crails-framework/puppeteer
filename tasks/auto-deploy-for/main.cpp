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
    ODB::id_type build_id = boost::lexical_cast<ODB::id_type>(argv[1]);

    task_params["build_id"] = build_id;
    Sidekic::async_task("auto_deploy", task_params.as_data());
    logger << Logger::Info << "Scheduled auto deploy for build " << build_id << Logger::endl;
  }
  else
  {
    logger << Logger::Info << "usage: " << argv[0] << " [build_id]" << Logger::endl;
    return -1;
  }
  return 0;
}
