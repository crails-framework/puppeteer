#ifndef  VIEW_CREDENTIAL_NEW_HPP
# define VIEW_CREDENTIAL_NEW_HPP

# include "../utility/model_form.hpp"
# include "app/comet/models/credential.hpp"
# include "html/views/credentials/new.hpp"

namespace Views
{
  class CredentialNew : public ModelForm<Front::Credential, HtmlTemplate::CredentialNew>
  {
  public:
    void initialize_breadcrumbs();

    std::string get_title() const { return has_model() ? model->get_name() : "New credentials"; }
    std::string get_credentials_name() const { return has_model() ? model->get_name() : ""; }
    std::string get_credentials_user() const { return has_model() ? model->get_username() : ""; }
    std::string get_credentials_pswd() const { return has_model() ? model->get_password() : ""; }

    void update_model_attributes();
  };
}

#endif
