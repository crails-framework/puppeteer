#include <map>
#include <crails/params.hpp>

using namespace std;
using namespace Crails;

void machine_upgrade(Params&);
void deploy_configuration(Params&);
void uninstall_configuration(Params&);
void deploy_build(Params&);
void auto_deploy(Params&);
void backup_sidetask(Params&);
void restore_sidetask(Params&);
void run_instance_script(Params&);

// Your tasks go here
map<string, function<void (Params&)> > sidetasks = {
  {"machine_upgrade",         machine_upgrade},
  {"deploy_configuration",    deploy_configuration},
  {"uninstall_configuration", uninstall_configuration},
  {"auto_deploy",             auto_deploy},
  {"deploy_build",            deploy_build},
  {"instance_script",         run_instance_script},
  {"backup",                  backup_sidetask},
  {"restore",                 restore_sidetask}
};
