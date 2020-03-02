#ifndef  SEARCHES_CONTROLLER_HPP
# define SEARCHES_CONTROLLER_HPP

# include "application_controller.hpp"

class SearchesController : public ApplicationController
{
  typedef ApplicationController Super;
public:
  SearchesController(Crails::Params&);

  void search();
private:
};

#endif
