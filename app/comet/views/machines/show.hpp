#ifndef  VIEW_MACHINE_HPP
# define VIEW_MACHINE_HPP

# include "../utility/model_view.hpp"
# include "app/comet/models/machine.hpp"
# include "app/comet/collections/instances.hpp"
# include "html/views/machines/show.hpp"
# include "app/comet/views/resources/elements/breadcrumbs.hpp"
# include "../instances/task_runner.hpp"

namespace Views
{
  class MachineShow : public ModelView<Front::Machine, HtmlTemplate::MachineShow>
  {
  public:
    MachineShow();
    virtual ~MachineShow() {}

    void initialize_breadcrumbs();
    void fetch_state();

  private:
    void on_model_received();
    void on_upgrade_clicked();
    void on_upgrade_task_progress(Comet::Object);

    TaskRunner task_runner;
    std::shared_ptr<Front::MachineInstances> instances;
  };
}

#endif
