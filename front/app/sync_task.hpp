#ifndef  SYNC_TASK_HPP
# define SYNC_TASK_HPP

# include "faye.hpp"

namespace Sync
{
  class Tasks
  {
  public:
    Tasks(Sync::Faye& _faye) : faye(_faye)
    {
      faye.subscribe("/tasks", std::bind(&Tasks::on_receive, this, std::placeholders::_1));
    }

    void listen_to(const std::string& id, std::function<void (Crails::Front::Object)> callback)
    {
      watchers.insert(std::pair<std::string, std::function<void (Crails::Front::Object)> >(id, callback));
    }

    void stop_listening(const std::string& id)
    {
      watchers.erase(id);
    }

  private:
    void on_receive(Crails::Front::Object data)
    {
      Crails::Front::ObjectImpl<client::String> data_string(*data);
      auto response = Crails::Front::Object::from_json(*data_string);
      std::string task_id = response["id"];

      for (const auto& entry : watchers)
      {
        if (entry.first == task_id)
        {
          entry.second(response);
          break ;
        }
      }
    }

    Sync::Faye& faye;
    std::map<std::string, std::function<void (Crails::Front::Object)> > watchers;
  };
}

#endif
