#include "with_nginx_machine.hpp"
#include "app/ssh/stream.hpp"
#include "app/ssh/session.hpp"

using namespace std;

void WithNginxMachine::reload_nginx(Sync::Stream& stream) const
{
  nginx_machine.open_ssh([&](Ssh::Session& ssh)
  {
    string command = "systemctl reload nginx";

    stream << command << '\n';
    ssh.exec(command, stream);
  });
}
