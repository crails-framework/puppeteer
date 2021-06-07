#ifndef  VIEW_MACHINE_NEW_HPP
# define VIEW_MACHINE_NEW_HPP

# include "../utility/model_form.hpp"
# include "html/views/machines/new.hpp"
# include "app/comet/models/machine.hpp"

namespace Views
{
  class MachineNew : public ModelForm<Front::Machine, HtmlTemplate::MachineNew>
  {
  public:
    MachineNew();
    virtual ~MachineNew() {}

    void initialize_breadcrumbs();

    std::string get_title()        const { return has_model() ? model->get_name() : "New machine"; }
    std::string get_machine_ip()   const { return has_model() ? model->get_ip() : ""; }
    std::string get_machine_name() const { return has_model() ? model->get_name() : ""; }

    void update_model_attributes();
  };
}

#endif
