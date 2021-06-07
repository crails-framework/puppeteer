#include "session.hpp"
#include "channel.hpp"
#include "scp.hpp"
#include <crails/logger.hpp>

using namespace std;
using namespace Ssh;

Session::Session()
{
  handle = ssh_new();
  ssh_set_blocking(handle, 1);
}

Session::~Session()
{
  std::cout << "Closing ssh session" << std::endl;
  if (handle != NULL)
    ssh_free(handle);
}

void Session::connect(const string& user, const string& ip, const string& port)
{
  ssh_options_set(handle, SSH_OPTIONS_HOST,          ip.c_str());
  ssh_options_set(handle, SSH_OPTIONS_PORT_STR,      port.c_str());
  ssh_options_set(handle, SSH_OPTIONS_USER,          user.c_str());
  ssh_options_set(handle, SSH_OPTIONS_LOG_VERBOSITY, &vbs);
  int con_result = ssh_connect(handle);
  if (con_result != SSH_OK)
  {
    std::cout << "SSH connection failed. Error code is:  " << con_result << std::endl;
    raise("SSH connection failed");
  }
}

void Session::authentify_with_pubkey(const string& password)
{
  int auth_result = ssh_userauth_publickey_auto(handle, NULL, password.c_str());
  if (auth_result != SSH_AUTH_SUCCESS)
  {
    std::cout << "SSH authentication failed. Error code is:  " << auth_result << std::endl;
    raise("SSH authentication failed");
  }
}

shared_ptr<Channel> Session::make_channel(int read_timeout)
{
  auto channel = make_shared<Channel>();

  channel->handle = ssh_channel_new(handle);
  channel->timeout_ms = read_timeout;
  if (channel == NULL)
  {
    std::cout << "Failed to create SSH channel." << std::endl;
    raise("Failed to create SSH channel");
  }

  ssh_channel_open_session(channel->handle);
  if (ssh_channel_is_open(channel->handle))
    std::cout << "Channel is open" << std::endl;
  else
    std::cout << "Channel is closed" << std::endl;
  return channel;
}

shared_ptr<Scp> Session::make_scp_session(const string& path, int mode)
{
  return make_shared<Scp>(handle, path, mode);
}

void Session::raise(const string& message)
{
  std::stringstream stream;

  stream << "Ssh::Session " << message << ": " << ssh_get_error(handle);
  throw std::runtime_error(stream.str().c_str());
}

int Session::exec(const string& command, Sync::Stream& output, int read_timeout)
{
  int retval = make_channel(read_timeout)->exec(command, output);

  if (retval == SSH_ERROR)
    Crails::logger << Crails::Logger::Error << "Ssh::Session::exec failed: " << get_error();
  return retval;
}

std::string Session::get_error()
{
  return ssh_get_error(handle);
}
