#include "scp.hpp"
#include <stdexcept>
#include <crails/read_file.hpp>

using namespace Ssh;
using namespace std;

Scp::Scp(ssh_session session_handle, const string& path, int mode) : session_handle(session_handle)
{
  handle = ssh_scp_new(session_handle, mode, path.c_str());
  if (handle == NULL)
    throw std::runtime_error("failed to create SCP session for path " + path);
}

Scp::~Scp()
{
  if (is_open)
    close();
  ssh_scp_free(handle);
}

void Scp::open()
{
  int rc = ssh_scp_init(handle);

  if (rc != SSH_OK)
  {
    string strerr = string("Ssh::Scp: failed to open SCP session: ") + ssh_get_error(session_handle);
    throw std::runtime_error(strerr);
  }
  is_open = true;
}

void Scp::close()
{
  ssh_scp_close(handle);
}

void Scp::push_directory(const string& path, int mode)
{
  int rc;

  require_opened_session();
  rc = ssh_scp_push_directory(handle, path.c_str(), mode);
  if (rc != SSH_OK)
    throw std::runtime_error("cannot remotely create directory " + path);
}

void Scp::push_file(const std::string& source, const std::string& target, int mode)
{
  string content;

  if (Crails::read_file(source, content))
    push_text(content, target, mode);
  else
    throw std::runtime_error("Ssh::Scp: cannot read local file " + source);
}

void Scp::push_text(const std::string& content, const std::string& target, int mode)
{
  int rc;

  require_opened_session();
  rc = ssh_scp_push_file(handle, target.c_str(), content.length(), mode);
  if (rc != SSH_OK)
    throw std::runtime_error("Ssh::Scp: cannot open remote file " + target);
  rc = ssh_scp_write(handle, content.c_str(), content.length());
  if (rc != SSH_OK)
    throw std::runtime_error("Ssh::Scp: cannot write remote file " + target);
}
