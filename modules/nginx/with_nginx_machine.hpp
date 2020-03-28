#ifndef  RECIPE_WITH_NGINX_MACHINE_HPP
# define RECIPE_WITH_NGINX_MACHINE_HPP

#include "app/models/recipe.hpp"
#include "app/models/machine.hpp"

struct WithNginxMachine
{
  mutable Machine nginx_machine;

  void reload_nginx(Sync::Stream&) const;
};

#endif
