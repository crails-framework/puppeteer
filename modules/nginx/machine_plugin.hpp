#ifndef  NGINX_MACHINE_PLUGIN_HPP
# define NGINX_MACHINE_PLUGIN_HPP

# include "app/models/machine.hpp"

struct MachineLetsencryptPlugin : public Machine::Plugin
{
  void upgrade(const Machine&, Sync::Stream&) const;
};

#endif
