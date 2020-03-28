#include "machine_plugin.hpp"
#include "app/ssh/session.hpp"

using namespace std;

void MachineLetsencryptPlugin::upgrade(const Machine& machine, Sync::Stream& stream) const
{
  machine.open_ssh([&](Ssh::Session& ssh)
  {
    Sync::Stream null_stream;
    bool         has_certbot;
    string       command = "certbot renew -n";

    has_certbot = ssh.exec("which certbot", null_stream) == 0;
    if (has_certbot)
    {
      stream << "\n# Renewing Let's Encrypt certificates\n";
      ssh.exec(command, stream);
    }
  });
}
