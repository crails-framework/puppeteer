#ifndef  FRONT_INSTANCE_HPP
# define FRONT_INSTANCE_HPP

# include "application.hpp"
# include "app/models/instance.hpp"
# include "app/models/variable_list.hpp"
# include "app/models/instance_state.hpp"

namespace Front
{
  class Instance : public ::Instance
  {
  public:
    model_class("instances")
    model_default_constructors(Instance)

    Comet::Signal<void> remote_state_changed;

    VariableList get_variable_list() const { return get_variables(); }
    void set_variable_list(const VariableList& list) { set_variables(list); }

    InstanceState state;
  };
}

#endif

