#ifndef  VIEW_MACHINE_NEW_HPP
# define VIEW_MACHINE_NEW_HPP

# include "utility/model_form.hpp"
# include "../app/machines.hpp"

namespace Views
{
  class MachineNew : public ModelForm<Puppeteer::Machine>
  {
    Crails::Front::Element input_name, input_ip;
  public:
    MachineNew() : ModelForm("New machine")
    {
      input_name = El("input", {{"name","machine_name"},{"type","text"},{"class","form-control"}});
      input_ip   = El("input", {{"name","machine_ip"},{"type","text"},{"class","form-control"}});
    }

    std::unordered_map<std::string, El> get_inputs()
    {
      return {
        {"name",       input_name},
        {"ip address", input_ip}
      };
    }

    void update_model_attributes()
    {
      model->set_name(input_name.get_value());
      model->set_ip(input_ip.get_value());
    }

    void update_form_attributes()
    {
      input_name.value(model->get_name());
      input_ip.value(model->get_ip());
    }

    void attached()
    {
      std::cout << "MachineNew attached" << std::endl;
      ModelForm::attached();
    }

    void on_saved(const Crails::Front::Ajax& ajax)
    {
      std::cout << "!!!" << std::endl;
      std::cout << "saving model success" << std::endl;
      std::cout << "Machine id = " << model->get_id() << std::endl;
      std::cout << "Machine name = " << model->get_name() << std::endl;
      std::cout << "!!!" << std::endl;
      ModelForm::on_saved(ajax);
    }

    void on_save_failed(const Crails::Front::Ajax&)
    {
      std::cout << "saving model failed" << std::endl;
    }
  };
}

#endif
