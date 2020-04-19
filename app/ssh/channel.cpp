#include "channel.hpp"
#include <crails/logger.hpp>

using namespace Crails;
using namespace Ssh;
using namespace std;

int Channel::exec(const string& command, Sync::Stream& output)
{
  bool is_eof         = false;
  bool read_timed_out = false;
  char buffer[256];
  int  bytes_read;
  int rc = ssh_channel_request_exec(handle, command.c_str());

  if (rc == SSH_OK)
  {
    is_eof = ssh_channel_is_eof(handle);
    logger << Logger::Info;
    logger << "Ssh::Channel ssh_channel_request_exec successfull" << Logger::endl;
    logger << "Ssh::Channel running command `" << command << "` (timeout in " << timeout_ms << ')' << Logger::endl;
    while (!is_eof && !read_timed_out)
    {
      switch (ssh_channel_poll_timeout(handle, timeout_ms, false)) {
      case SSH_EOF:
        is_eof = true;
        logger << "Ssh::Channel command returned successfully" << Logger::endl;
        break ;
      case 0:
        read_timed_out = true;
        logger << "Ssh::Channel ssh_channel_read_timeout timed out" << Logger::endl;
        break ;
      default:
        {
          bytes_read = ssh_channel_read_timeout(handle, buffer, sizeof(buffer), 0, timeout_ms);
          for (int i = 0 ; i < bytes_read ; ++i)
            output << buffer[i];
          is_eof = bytes_read == 0 || ssh_channel_is_eof(handle);
        }
        break ;
      }
    }
  }
  else
    logger << Logger::Error << "Ssh::Channel: ssh_channel_request_exec returned with status " << rc << Logger::endl;
  logger << Logger::Info << "Ssh::Channel ssh_channel_request_exec ended" << Logger::endl;
  return is_eof ? ssh_channel_get_exit_status(handle) : -1;
}

Channel::~Channel()
{
  if (handle)
  {
    ssh_channel_close(handle);
    ssh_channel_free(handle);
  }
}
