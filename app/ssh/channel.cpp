#include "channel.hpp"

using namespace Ssh;
using namespace std;

Channel::Channel(ssh_session session_handle)
{
  handle = ssh_channel_new(session_handle);
  if (handle == NULL)
    throw std::runtime_error("cannot create an ssh channel");
}

Channel::~Channel()
{
  if (handle != NULL)
  {
    close();
    ssh_channel_free(handle);
  }
}

void Channel::open()
{
  if (!is_open)
  {
    int rc = ssh_channel_open_session(handle);

    if (rc != SSH_OK)
      throw std::runtime_error("cannot open an ssh channel");
    is_open = true;
  }
}

void Channel::close()
{
  if (is_open)
  {
    ssh_channel_close(handle);
    is_open = false;
  }
}

int Channel::exec(const std::string& command, Sync::Stream& output)
{
  char buffer[256];
  int  nbytes;
  int rc;

  open();
  rc = ssh_channel_request_exec(handle, command.c_str());
  if (rc != SSH_OK)
    throw std::runtime_error("cannot run command `" + command + "` on ssh channel");
  do {
    nbytes = ssh_channel_read(handle, buffer, sizeof(buffer), 0);
    for (int i = 0 ; i < nbytes ; ++i)
      output << buffer[i];
  } while (nbytes > 0);
  if (nbytes < 0)
    throw std::runtime_error("failed to run command `" + command + "` on ssh channel");
  ssh_channel_send_eof(handle);
  return ssh_channel_get_exit_status(handle);
}

int Channel::exec(const std::string& command)
{
  Sync::Stream stream;

  return exec(command, stream);
}
