#ifndef  VIEW_MACHINE_HPP
# define VIEW_MACHINE_HPP

# include "utility/model_view.hpp"
# include "../app/machines.hpp"
# include "lib/cheerp-html/views/machine.hpp"
# include "front/resources/elements/breadcrumbs.hpp"

namespace Views
{
  class Machine : public ModelView<Puppeteer::Machine, HtmlTemplate::Machine>
  {
  public:
    Machine()
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
