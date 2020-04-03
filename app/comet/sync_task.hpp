#ifndef  SYNC_TASK_HPP
# define SYNC_TASK_HPP

# include "faye.hpp"

namespace Sync
{
  class Tasks
  {
    typedef std::function<void (Comet::Object)> Callback;
    typedef std::pair<std::string, Callback>    Listener;
  public:
    typedef std::list<Listener>::iterator ListenerId;

    Tasks(Sync::Faye&);

    ListenerId listen_to(const std::string& id, Callback callback);
    void       stop_listening(ListenerId);

  private:
    void on_receive(Comet::Object data);

    Sync::Faye& faye;
    std::list<Listener> watchers;
  };
}

#endif
