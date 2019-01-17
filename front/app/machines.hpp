#ifndef  FRONT_MACHINES_HPP
# define FRONT_MACHINES_HPP

# include "model.hpp"
# include "app/models/machine.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Machine : public ::Machine
  {
  public:
    model_class("machines")
    model_default_constructors(Machine)
  };

  class Machines : public Crails::Front::Collection<Machine>
  {
  public:
    std::string get_url() const
    {
      return '/' + Machine::get_scope();
    }
  };
}

#endif
