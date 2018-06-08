#include <map>
#include <crails/params.hpp>

using namespace std;
using namespace Crails;

void deploy_configuration(Params&);
void uninstall_configuration(Params&);

// Your tasks go here
map<string, function<void (Params&)> > sidetasks = {
  {"deploy_configuration",    deploy_configuration},
  {"uninstall_configuration", uninstall_configuration}
};
