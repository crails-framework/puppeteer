#include "channel.hpp"

using namespace Ssh;
using namespace std;

int Channel::exec(const string& command, Sync::Stream& output)
{
  int  timeout_ms = 1000;
  char buffer[256];
  int  bytes_red;
  int rc = ssh_channel_request_exec(handle, command.c_str());

  while (!ssh_channel_is_eof(handle))
  {
    bytes_red = ssh_channel_read_timeout(handle, buffer, sizeof(buffer), 0, timeout_ms);

    for (int i = 0 ; i < bytes_red ; ++i)
      output << buffer[i];
    std::cout << buffer << std::endl;
  }
  return ssh_channel_get_exit_status(handle);
}

Channel::~Channel()
{
  if (handle)
  {
    ssh_channel_close(handle);
    ssh_channel_free(handle);
  }
}
