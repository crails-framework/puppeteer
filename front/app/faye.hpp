#ifndef  FAYE_HPP
# define FAYE_HPP

# include <iostream>
# include <string>
# include <crails/front/globals.hpp>

namespace client
{
  class FayeClient
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

    void subscribe(const std::string& path, std::function<void (Crails::Front::Object)> callback)
    {
      client->subscribe(path.c_str(), cheerp::Callback([callback](client::Object* raw_object)
      {
        callback(Crails::Front::Object(raw_object));
      }));
    }

    static std::string get_url()
    {
      auto* location = Crails::Front::window->get_location();
      std::string protocol = Crails::Front::Object(location->get_protocol());
      std::string hostname = Crails::Front::Object(location->get_hostname());
      std::string port     = Crails::Front::Object(location->get_port());

      if (hostname == "localhost")
        port = "9292";
      return protocol + "//" + hostname + ':' + port + "/faye";
    }

  private:
    client::FayeClient* client;
  };
}

#endif
