#include "app/models/machine.hpp"
#include "app/ssh/stream.hpp"

#ifdef WITH_NGINX_PLUGIN
# include "modules/nginx/machine_plugin.hpp"
#endif

using namespace std;

struct EmptyMachinePlugin : public Machine::Plugin
{
  void upgrade(const Machine&, Sync::Stream&) const { }
};

const vector<shared_ptr<Machine::Plugin> > Machine::plugins = {
#ifdef WITH_NGINX_PLUGIN
  make_shared<MachineLetsencryptPlugin>(),
#endif
  make_shared<EmptyMachinePlugin>()
};
