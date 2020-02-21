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
    std::string get_title()        const { return model ? model->get_name() : "New machine"; }
    std::string get_machine_ip()   const { return model ? model->get_ip() : ""; }
    std::string get_machine_name() const { return model ? model->get_name() : ""; }

    void update_model_attributes()
    {
      auto name_input = find("[name=\"machine_name\"]")[0];
      auto ip_input   = find("[name=\"machine_ip\"]")[0];

      model->set_name(name_input.get_value());
      model->set_ip(ip_input.get_value());
    }

    void on_saved()
    {
      std::cout << "!!!" << std::endl;
      std::cout << "saving model success" << std::endl;
      std::cout << "Machine id = " << model->get_id() << std::endl;
      std::cout << "Machine name = " << model->get_name() << std::endl;
      std::cout << "!!!" << std::endl;
      ModelForm::on_saved();
    }

    void on_save_failed()
    {
      std::cout << "saving model failed" << std::endl;
    }
  };
}

#endif
