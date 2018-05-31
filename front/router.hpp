#ifndef  FRONT_ROUTER_HPP
# define FRONT_ROUTER_HPP

# include <crails/front/router.hpp>

namespace Puppeteer
{
  class Router : public Crails::Front::Router
  {
  public:
    void initialize();

    static Router* instance;
  };
}

#endif
