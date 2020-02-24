#include <map>
#include <crails/params.hpp>

using namespace std;
using namespace Crails;

void deploy_configuration(Params&);
void uninstall_configuration(Params&);
void deploy_build(Params&);
void auto_deploy(Params&);
void run_instance_script(Params&);

// Your tasks go here
map<string, function<void (Params&)> > sidetasks = {
  {"deploy_configuration",    deploy_configuration},
  {"uninstall_configuration", uninstall_configuration},
  {"auto_deploy",             auto_deploy},
  {"deploy_build",            deploy_build},
  {"instance_script",         run_instance_script}
};
