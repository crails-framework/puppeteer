#ifndef  FRONT_MACHINE_HPP
# define FRONT_MACHINE_HPP

# include "application.hpp"
# include "app/models/machine.hpp"

namespace Front
{
  class Machine : public ::Machine
  {
  public:
    model_class("machines")
    model_default_constructors(Machine)
  };
}

#endif
