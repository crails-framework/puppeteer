#ifndef  VIEW_INSTANCE_HPP
# define VIEW_INSTANCE_HPP

# include "utility/model_view.hpp"
# include "utility/button.hpp"
# include <iostream>
# include "../app/instances.hpp"

# include "instance/action_widget.hpp"
# include "instance/state_widget.hpp"
# include "instance/console_output.hpp"

# include "lib/cheerp-html/views/instance.hpp"

namespace Views
{
  class Instance : public ModelView<Puppeteer::Instance, HtmlTemplate::Instance>
  {
  public:
    Instance()
    {
      action_widget.set_console_output(&console_output);
    }

    void activate(unsigned long instance_id)
    {
      ModelView::activate(instance_id);
    }
  };
}

#endif
