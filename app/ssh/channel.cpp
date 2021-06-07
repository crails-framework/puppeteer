#include "channel.hpp"
#include <crails/logger.hpp>

using namespace Crails;
using namespace Ssh;
using namespace std;

int Channel::exec(const string& command, Sync::Stream& output)
{
  bool is_eof = false;
  char buffer[256];
  int  bytes_read;
  int rc = ssh_channel_request_exec(handle, command.c_str());
  int foo = 0;


  if (rc == SSH_OK)
  {
    while (ssh_channel_is_open(handle) && !(is_eof = ssh_channel_is_eof(handle)))
    {
      bytes_read = ssh_channel_read_timeout(handle, buffer, sizeof(buffer), 0, 600000 /*timeout_ms*/);
      if (bytes_read < 0)
      {
        foo++;
        logger << Logger::Error << "Ssh::Channel: ssh_channel_read_timeout returned: " << bytes_read << Logger::endl;
	is_eof = ssh_channel_is_eof(handle);
	if (foo > 1000)
          return SSH_ERROR;
      }
      if (bytes_read > 0)
      {
        foo = 0;
        for (int i = 0 ; i < bytes_read ; ++i)
          output << buffer[i];
      }
      else
      {
        foo++;
        logger << Logger::Error << "Ssh::Channel: ssh_channel_read_timeout timed out" << Logger::endl;
	is_eof = ssh_channel_is_eof(handle);
	if (foo > 1000)
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
