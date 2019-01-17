#ifndef  FRONT_BUILDS_HPP
# define FRONT_BUILDS_HPP

# include "app/models/build.hpp"
# include "model.hpp"
# include "app/models/variable_list.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Build : public ::Build
  {
  public:
    model_class("builds")
    model_default_constructors(Build)

    VariableList get_variable_list() const { return get_variables(); }
    void set_variable_list(const VariableList& list) { set_variables(list); }
  };

  class Builds : public Crails::Front::Collection<Build>
  {
  public:
    std::string get_url() const
    {
      return '/' + Build::get_scope();
    }
  };
}

#endif
