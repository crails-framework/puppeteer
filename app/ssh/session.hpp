#ifndef  SSH_SESSION_HPP
# define SSH_SESSION_HPP

#include <libssh/libssh.h>
#include <memory>

namespace Ssh
{
  class Channel;
  class Scp;

  class Session
  {
  public:
    Session();
    ~Session();

    void should_accept_unknown_hosts(bool val);

    void disconnect();
    void connect(const std::string& username, const std::string& hostname, int port = 22);

    void authentify_host();
    void authentify(const std::string& password);
    void authentify_with_pubkey(const std::string& password = "");

    int exec(const std::string& command, std::ostream&);

    std::shared_ptr<Channel> make_channel();
    std::shared_ptr<Scp>     make_scp_session(const std::string& path, int mode);

  private:
    void raise(const std::string& message);

    ssh_session handle;
    int verbosity;
    int rc;
    bool authentified = false;
    bool accepts_unknown_hosts = false;
  };
}

#endif
