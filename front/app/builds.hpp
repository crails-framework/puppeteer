#ifndef  FRONT_BUILDS_HPP
# define FRONT_BUILDS_HPP

# include "data/build.hpp"
# include "model.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Build : public BuildData
  {
  public:
    model_class("builds")
    model_default_constructors(Build)
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
