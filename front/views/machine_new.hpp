#ifndef  VIEW_MACHINE_NEW_HPP
# define VIEW_MACHINE_NEW_HPP

# include "utility/model_form.hpp"
# include "lib/cheerp-html/views/machine_new.hpp"
# include "../app/machines.hpp"

namespace Views
{
  class MachineNew : public ModelForm<Puppeteer::Machine, HtmlTemplate::MachineNew>
  {
  public:
    void initialize_breadcrumbs();

    std::string get_title()        const { return model ? model->get_name() : "New machine"; }
    std::string get_machine_ip()   const { return model ? model->get_ip() : ""; }
    std::string get_machine_name() const { return model ? model->get_name() : ""; }

    void update_model_attributes();
  };
}

#endif
