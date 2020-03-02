#ifndef  VIEW_INSTANCE_HPP
# define VIEW_INSTANCE_HPP

# include "utility/model_view.hpp"
# include "utility/button.hpp"
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
    Instance();

    void initialize_breadcrumbs();
    void activate(unsigned long instance_id);
    void on_model_received();
    void on_action_state_update(bool is_busy);

    std::string get_backup_path() const;
  };
}

#endif
