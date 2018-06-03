#ifndef  FRONT_INSTANCES_HPP
# define FRONT_INSTANCES_HPP

# include "model.hpp"
# include "data/instance.hpp"
# include "app/models/variable_list.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Instance : public InstanceData
  {
  public:
    model_class("instances")
    model_default_constructors(Instance)

    VariableList get_variable_list() const { return get_variables(); }
    void set_variable_list(const VariableList& list) { set_variables(list); }
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

