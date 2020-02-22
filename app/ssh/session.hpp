#ifndef  SSH_SESSION_HPP
# define SSH_SESSION_HPP

#include <libssh/libssh.h>
#include <memory>
#include "stream.hpp"

namespace Ssh
{
  class Channel;
  class Scp;

  struct Session
  {
    friend class Channel;
    ssh_session handle;
    int vbs = SSH_LOG_RARE;
    bool accepts_unknown_hosts = false;
    bool is_open = false;
  public:
    Session();
    ~Session();

    void should_accept_unknown_hosts(bool val) { accepts_unknown_hosts = val; }

    void connect(const std::string& user, const std::string& ip, const std::string& port = "22");
    void authentify_with_pubkey(const std::string& password = "");
    std::shared_ptr<Channel> make_channel();
    std::shared_ptr<Scp>     make_scp_session(const std::string& path, int mode);
    void raise(const std::string& message);

    int exec(const std::string& command, Sync::Stream& output);

  private:
  };
}

# include "channel.hpp"
# include "scp.hpp"

#endif
