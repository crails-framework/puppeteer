#ifndef  VIEW_INSTANCE_HPP
# define VIEW_INSTANCE_HPP

# include "utility/model_view.hpp"
# include "utility/button.hpp"
# include <iostream>
# include "../app/instances.hpp"
# include "../app/backups.hpp"

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
      state_widget.set_progress_bar(action_widget.progress_bar);
      action_widget.set_console_output(&console_output);
      state_widget .set_console_output(&console_output);
      action_widget.performing_action_signal.connect(std::bind(&Instance::on_action_state_update, this, std::placeholders::_1));
      state_widget .performing_action_signal.connect(std::bind(&Instance::on_action_state_update, this, std::placeholders::_1));
    }

    void activate(unsigned long instance_id)
    {
      ModelView::activate(instance_id);
    }

    void on_model_received()
    {
      ModelView::on_model_received();
      model->remote_state_changed.connect([this]()
      {
        signaler.trigger("state-changed");
      });
    }

    void on_action_state_update(bool is_busy)
    {
      auto buttons = find(".actions .au-btn, .build-deploy-ui .au-btn");

      for (auto button : buttons)
        button.toggle_class("disabled", is_busy);
    }

    std::string get_backup_path() const
    {
      return Puppeteer::Backup::get_path_for_instance(model->get_id());
    }
  };
}

#endif
