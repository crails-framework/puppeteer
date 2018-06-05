#ifndef  VIEW_CREDENTIAL_NEW_HPP
# define VIEW_CREDENTIAL_NEW_HPP

# include "utility/model_form.hpp"
# include "../app/credentials.hpp"

namespace Views
{
  class CredentialNew : public ModelForm<Puppeteer::Credential>
  {
    Crails::Front::Element input_name, input_username, input_password;
  public:
    CredentialNew() : ModelForm("New credentials")
    {
      input_name     = El("input", {{"name","credential_name"},{"type","text"},{"class","form-control"}});
      input_username = El("input", {{"name","credential_user"},{"type","text"},{"class","form-control"}});
      input_password = El("input", {{"name","credential_pswd"},{"type","password"},{"class","form-control"}});
    }

    std::unordered_map<std::string, El> get_inputs()
    {
      return {
        {"title",    input_name},
        {"username", input_username},
        {"password", input_password}
      };
    }

    void update_model_attributes()
    {
      model->set_name(input_name.get_value());
      model->set_username(input_username.get_value());
      model->set_password(input_password.get_value());
    }

    void update_form_attributes()
    {
      input_name.value(model->get_name());
      input_username.value(model->get_username());
      input_password.value(model->get_password());
    }
  };
}

#endif
