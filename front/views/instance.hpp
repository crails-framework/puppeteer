#ifndef  VIEW_INSTANCE_HPP
# define VIEW_INSTANCE_HPP

# include "utility/model_view.hpp"
# include "utility/button.hpp"
# include <iostream>
# include "../app/instances.hpp"

# include "instance/action_widget.hpp"
# include "instance/state_widget.hpp"

namespace Views
{
  class Instance : public ModelView<Puppeteer::Instance>
  {
    InstanceStateWidget  state_widget;
    InstanceActionWidget action_widget;
  public:
    Instance() : ModelView("Instance")
    {
      page_content.attr("class", "row").inner({
        El("div", {{"class","col-lg-6"}}).inner({
          Theme::card("State", state_widget)
        }),
        El("div", {{"class","col-lg-6"}}).inner({
          Theme::card("Actions", action_widget)
        })
      });
    }

    void activate(unsigned long instance_id)
    {
      ModelView::activate(instance_id);
    }

    void on_model_received()
    {
      state_widget.activate(model);
      action_widget.activate(model);
    }
  };
}

#endif
