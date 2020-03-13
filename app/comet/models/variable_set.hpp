#ifndef  FRONT_VARIABLE_SET_HPP
# define FRONT_VARIABLE_SET_HPP

# include "application.hpp"
# include "app/models/variable_set.hpp"
# include "app/models/variable_list.hpp"

namespace Front
{
  class VariableSet : public ::VariableSet
  {
  public:
    model_default_constructors(VariableSet)
    static std::string get_scope() { return "variable_sets"; }

    std::string get_url() const
    {
      return "/variables";
    }

    std::string get_path() const
    {
      return "#/variables";
    }
  };
}

#endif

