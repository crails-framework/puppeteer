#ifndef  FRONT_INSTANCES_HPP
# define FRONT_INSTANCES_HPP

# include "model.hpp"
# include "data/instance.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Instance : public InstanceData
  {
  public:
    model_class("instances")
    model_default_constructors(Instance)
  };

  class Instances : public Crails::Front::Collection<Instance>
  {
  public:
    std::string get_url() const
    {
      return '/' + Instance::get_scope();
    }
  };
}

#endif

