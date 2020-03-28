#include "machine_plugin.hpp"
#include "app/ssh/session.hpp"

using namespace std;

void MachineLetsencryptPlugin::upgrade(const Machine& machine, Sync::Stream& stream) const
{
  machine.open_ssh([&](Ssh::Session& ssh)
  {
    string command = "certbot renew -n";

    ssh.exec(command, stream);
  });
}
