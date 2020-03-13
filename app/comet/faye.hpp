#ifndef  FAYE_HPP
# define FAYE_HPP

# include <iostream>
# include <string>
# include <comet/globals.hpp>

namespace client
{
  class FayeClient : public client::Object
  {
  public:
    FayeClient(const String&);

    void             disable(const String&);
    void             subscribe(const String&, EventListener*);
    client::Promise* publish(const String&, Object*);
  };
}

namespace Sync
{
  class Faye
  {
  public:
    Faye()
    {
      client = new client::FayeClient(get_url().c_str());
      client->disable("eventsource");
      client->disable("websocket");
    }

    void subscribe(const std::string& path, std::function<void (Comet::Object)> callback)
    {
      client->subscribe(path.c_str(), cheerp::Callback([callback](client::Object* raw_object)
      {
        callback(Comet::Object(raw_object));
      }));
    }

    static std::string get_url()
    {
      auto* location = Comet::window->get_location();
      std::string protocol = Comet::Object(location->get_protocol());
      std::string hostname = Comet::Object(location->get_hostname());
      std::string port     = Comet::Object(location->get_port());

      if (hostname == "localhost")
        port = "9292";
      return protocol + "//" + hostname + ':' + port + "/faye";
    }

  private:
    client::FayeClient* client;
  };
}

#endif
