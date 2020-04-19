#ifndef  SSH_CHANNEL_HPP
# define SSH_CHANNEL_HPP

# include <libssh/libssh.h>
# include <string>
# include "stream.hpp"

namespace Ssh
{
  class Session;

  class Channel
  {
    friend class Session;
    ssh_channel handle;
    int timeout_ms;
  public:
    ~Channel();

    int exec(const std::string& command, Sync::Stream& output);
  };
}

#endif
