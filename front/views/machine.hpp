#ifndef  VIEW_MACHINE_HPP
# define VIEW_MACHINE_HPP

# include "utility/model_view.hpp"
# include "../app/machines.hpp"

namespace Views
{
  class Machine : public ModelView<Puppeteer::Machine>
  {
  public:
    Machine() : ModelView("Machine")
    {
    }
  };
}

#endif
