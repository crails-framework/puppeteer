#ifndef  FRONT_VARIABLE_SETS_HPP
# define FRONT_VARIABLE_SETS_HPP

# include <crails/odb/id_type.hpp>
# include "model.hpp"
# include "app/models/variable_set.hpp"
# include "app/models/variable_list.hpp"
# include <crails/front/mvc/collection.hpp>

namespace Puppeteer
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
