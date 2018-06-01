#include "session.hpp"
#include "channel.hpp"
#include "scp.hpp"

using namespace std;
using namespace Ssh;

Session::Session()
{
  handle = ssh_new();
  if (handle == NULL)
    throw std::runtime_error("cannot start a new ssh session");
  verbosity = SSH_LOG_PROTOCOL;
}

Session::~Session()
{
  disconnect();
  if (handle != NULL)
    ssh_free(handle);
}

void Session::should_accept_unknown_hosts(bool val) { accepts_unknown_hosts = val; }

void Session::disconnect()
{
  if (rc == SSH_OK)
  {
    ssh_disconnect(handle);
    rc = SSH_ERROR;
    authentified = false;
  }
}

void Session::connect(const std::string& username, const std::string& hostname, int port)
{
  disconnect();
  ssh_options_set(handle, SSH_OPTIONS_HOST, hostname.c_str());
  ssh_options_set(handle, SSH_OPTIONS_PORT, &port);
  ssh_options_set(handle, SSH_OPTIONS_USER, username.c_str());
  ssh_options_set(handle, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  rc = ssh_connect(handle);
  if (rc != SSH_OK)
    raise("failed to connect");
  else
    authentify_host();
}

void Session::authentify_host()
{
  int state, hlen;
  unsigned char* hash = NULL;

  state = ssh_is_server_known(handle);
  hlen  = ssh_get_pubkey_hash(handle, &hash);
  switch (state)
  {
  case SSH_SERVER_KNOWN_OK:
    break ;
  case SSH_SERVER_KNOWN_CHANGED:
    raise("host key for server changed");
    break ;
  case SSH_SERVER_FOUND_OTHER:
    raise("the host key for this server was not found, but another type of key exists");
    break ;
  case SSH_SERVER_FILE_NOT_FOUND:
  case SSH_SERVER_NOT_KNOWN:
    if (accepts_unknown_hosts)
      ssh_write_knownhost(handle);
    else
      raise("host is unknown");
    break ;
  }
}

void Session::authentify(const std::string& password)
{
  int result = ssh_userauth_password(handle, NULL, password.c_str());

  if (result == SSH_AUTH_ERROR)
    raise("failed to authentify");
  authentified = true;
}

void Session::authentify_with_pubkey(const std::string& password)
{
  int result = ssh_userauth_publickey_auto(handle, NULL, password.c_str());

  if (result == SSH_AUTH_PARTIAL)
    authentify(password);
  else if (result == SSH_AUTH_ERROR)
    raise("failed to authentify");
  authentified = true;
}

std::shared_ptr<Channel> Session::make_channel()
{
  return std::make_shared<Channel>(handle);
}

std::shared_ptr<Scp> Session::make_scp_session(const std::string& path, int mode)
{
  return std::make_shared<Scp>(handle, path, mode);
}

int Session::exec(const string& command, ostream& output)
{
  return make_channel()->exec(command, output);
}

void Session::raise(const std::string& message)
{
  std::stringstream stream;

  stream << "Ssh::Session " << message << ": " << ssh_get_error(handle);
  throw std::runtime_error(stream.str().c_str());
}
