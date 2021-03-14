#ifndef  VIEW_INSTANCE_HPP
# define VIEW_INSTANCE_HPP

# include "../utility/model_view.hpp"
# include "app/comet/models/instance.hpp"

# include "_action_widget.hpp"
# include "_state_widget.hpp"
# include "_console_output.hpp"

# include "html/views/instances/show.hpp"

namespace Views
{
  class InstanceShow : public ModelView<Front::Instance, HtmlTemplate::InstanceShow>
  {
  public:
    InstanceShow();
    virtual ~InstanceShow() {}

    void initialize_breadcrumbs();
    void on_model_received();
    void on_action_state_update(bool is_busy);

    std::string get_backup_path() const;
  };
}

#endif
