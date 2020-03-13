#ifndef  FRONT_BUILD_HPP
# define FRONT_BUILD_HPP

# include "application.hpp"
# include "app/models/build.hpp"
# include "app/models/variable_list.hpp"

namespace Front
{
  class Build : public ::Build
  {
  public:
    model_class("builds")
    model_default_constructors(Build)

    VariableList get_variable_list() const { return get_variables(); }
    void set_variable_list(const VariableList& list) { set_variables(list); }

    static std::string get_available_builds_url_for(ODB::id_type id)
    {
      std::stringstream url;
      url << "/builds/" << id << "/available-builds";
      return url.str();
    }
  };
}

#endif
