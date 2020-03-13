#ifndef  VARIABLES_CONTROLLER_HPP
# define VARIABLES_CONTROLLER_HPP

# include "application.hpp"
# include "app/comet/models/variable_set.hpp"

class VariablesController : public ApplicationController
{
public:
  VariablesController(const Comet::Params&);

  void show();
};

#endif
