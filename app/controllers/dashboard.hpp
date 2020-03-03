#ifndef  DASHBOARD_CONTROLLER_HPP
# define DASHBOARD_CONTROLLER_HPP

# include "application_controller.hpp"

class DashboardController : public ApplicationController
{
  typedef ApplicationController Super;
public:
  DashboardController(Crails::Params&);

  void last_builds();
private:
};

#endif
