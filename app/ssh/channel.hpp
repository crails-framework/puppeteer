#ifndef  SSH_CHANNEL_HPP
# define SSH_CHANNEL_HPP

# include <libssh/libssh.h>
# include <string>
# include <sstream>

namespace Ssh
{
  class Channel
  {
  public:
    Channel(ssh_session session_handle);
    ~Channel();

    void open();
    void close();
    int exec(const std::string& command, std::ostream& output);
    int exec(const std::string& command);

  private:
    ssh_channel handle;
    bool is_open = false;
  };
}

#endif
