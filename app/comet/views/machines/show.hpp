#ifndef  VIEW_MACHINE_HPP
# define VIEW_MACHINE_HPP

# include "../utility/model_view.hpp"
# include "app/comet/models/machine.hpp"
# include "html/views/machines/show.hpp"
# include "front/resources/elements/breadcrumbs.hpp"

namespace Views
{
  class MachineShow : public ModelView<Front::Machine, HtmlTemplate::MachineShow>
  {
  public:
    MachineShow()
    {
      trigger_binding_updates();
    }

    void initialize_breadcrumbs()
    {
      if (model)
      {
        Breadcrumbs::reset();
        Breadcrumbs::set_machine(model->get_id());
        Breadcrumbs::done();
      }
    }
  };
}

#endif
