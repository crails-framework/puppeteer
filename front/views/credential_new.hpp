#ifndef  VIEW_CREDENTIAL_NEW_HPP
# define VIEW_CREDENTIAL_NEW_HPP

# include "utility/model_form.hpp"
# include "../app/credentials.hpp"

# include "lib/cheerp-html/views/credential_new.hpp"

namespace Views
{
  class CredentialNew : public ModelForm<Puppeteer::Credential, HtmlTemplate::CredentialNew>
  {
  public:
    std::string get_title() const { return model ? model->get_name() : "New credentials"; }
    std::string get_credentials_name() const { return model ? model->get_name() : ""; }
    std::string get_credentials_user() const { return model ? model->get_username() : ""; }
    std::string get_credentials_pswd() const { return model ? model->get_password() : ""; }

    void update_model_attributes()
    {
      auto name_input  = find("[name=\"credentials_name\"]")[0];
      auto user_input  = find("[name=\"credentials_user\"")[0];
      auto pswd_input  = find("[name=\"credentials_pswd\"")[0];

      model->set_name(name_input.get_value());
      model->set_username(name_input.get_value());
      model->set_password(pswd_input.get_value());
    }
  };
}

#endif
