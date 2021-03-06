#ifndef  FRONT_LOCAL_STORAGE_HPP
# define FRONT_LOCAL_STORAGE_HPP

# include "from_string.hpp"
# include "signal.hpp"
# include <cheerp/clientlib.h>

namespace Comet
{
  class LocalStorage
  {
  public:
    LocalStorage();
    LocalStorage(client::Storage*);
    ~LocalStorage();

    struct Change
    {
      Change(client::StorageEvent*);
      Change() {}
      std::string key, old_value, new_value;
    };

    Signal<Change> on_change;

    template<typename T>
    void set_item(const std::string& key, T value)
    {
      set_item<const std::string&>(key, std::to_string(value));
    }

    template<typename T>
    T get_item(const std::string& key) const
    {
      return Comet::from_string<T>(get_item<std::string>(key));
    }

    void remove_item(const std::string& key);
    void clear();

  private:
    void initialize();

    client::Storage*       storage;
    client::EventListener* event_listener;
  };

  template<> void LocalStorage::set_item<const std::string&>(const std::string& key, const std::string& value);
  template<> void LocalStorage::set_item<std::string>(const std::string& key, std::string value);
  template<> std::string LocalStorage::get_item<std::string>(const std::string& key) const;
}

#endif
