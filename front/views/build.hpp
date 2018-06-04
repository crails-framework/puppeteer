#ifndef  VIEW_BUILD_HPP
# define VIEW_BUILD_HPP

# include "utility/model_view.hpp"
# include "../app/builds.hpp"

namespace Views
{
  class Build : public ModelView<Puppeteer::Build>
  {
  public:
    Build() : ModelView("Build")
    {
    }
  };
}

#endif
