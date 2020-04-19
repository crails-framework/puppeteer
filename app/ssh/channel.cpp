#include "channel.hpp"
#include <crails/logger.hpp>

using namespace Crails;
using namespace Ssh;
using namespace std;

int Channel::exec(const string& command, Sync::Stream& output)
{
  int  timeout_ms = 1000;
  char buffer[256];
  int  bytes_read;
  int rc = ssh_channel_request_exec(handle, command.c_str());

  if (rc == SSH_OK)
  {
    bool is_eof = ssh_channel_is_eof(handle);

    logger << Logger::Info;
    logger << "Ssh::Channel ssh_channel_request_exec successfull" << Logger::endl;
    logger << "Ssh::Channel running command `" << command << '`' << Logger::endl;
    while (!is_eof)
    {
      bytes_read = ssh_channel_read_timeout(handle, buffer, sizeof(buffer), 0, timeout_ms);

      if (bytes_read > 0)
      {
        std::cout << "Channel: reading " << bytes_read << " bytes -> `";
        for (int i = 0 ; i < bytes_read ; ++i)
          output << buffer[i];
        std::cout << '`' << std::endl;
      }
      else
        logger << "Channel: ssh_channel_read returned " << bytes_read << Logger::endl;
      is_eof = bytes_read <= 0 || ssh_channel_is_eof(handle);
    }
    logger << "Ssh::Channel ssh_channel_request_exect ended" << Logger::endl;
    return ssh_channel_get_exit_status(handle);
  }
  else
    logger << Logger::Error << "Ssh::Channel: ssh_channel_request_exec returned with status " << rc << Logger::endl;
  logger << Logger::Info << "Ssh::Channel ssh_channel_request_exect ended" << Logger::endl;
  return -1;
}

Channel::~Channel()
{
  if (handle)
  {
    ssh_channel_close(handle);
    ssh_channel_free(handle);
  }
}
