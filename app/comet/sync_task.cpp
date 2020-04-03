#include "sync_task.hpp"

using namespace Sync;
using namespace std;

Tasks::Tasks(Faye& _faye) : faye(_faye)
{
  faye.subscribe("/tasks", std::bind(&Tasks::on_receive, this, placeholders::_1));
}

Sync::Tasks::ListenerId Tasks::listen_to(const string& id, Tasks::Callback callback)
{
  return watchers.insert(watchers.end(), Listener(id, callback));
}

void Tasks::stop_listening(ListenerId it)
{
  watchers.erase(it);
}

void Tasks::on_receive(Comet::Object data)
{
  vector<ListenerId> iterators;
  Comet::ObjectImpl<client::String> data_string(*data);
  auto response = Comet::Object::from_json(*data_string);
  string task_id = response["id"];

  for (auto it = watchers.begin() ; it != watchers.end() ; ++it)
  {
    if (it->first == task_id)
      iterators.push_back(it);
  }
  for (ListenerId iterator : iterators)
    iterator->second(response);
}
