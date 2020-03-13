#ifndef  DASHBOARD_CONTROLLER_HPP
# define DASHBOARD_CONTROLLER_HPP

# include "application.hpp"

class DashboardController : public ApplicationController
{
public:
  DashboardController(const Comet::Params&);

  void index();
};

#endif
