#ifndef  VIEW_CREDENTIAL_HPP
# define VIEW_CREDENTIAL_HPP

# include "../utility/model_view.hpp"
# include "app/comet/models/credential.hpp"
# include "app/comet/views/resources/elements/breadcrumbs.hpp"
# include "html/views/credentials/show.hpp"

namespace Views
{
  class CredentialShow : public ModelView<Front::Credential, HtmlTemplate::CredentialShow>
  {
    void initialize_breadcrumbs()
    {
      if (model)
      {
        Breadcrumbs::reset();
        Breadcrumbs::set_credential(model->get_id());
        Breadcrumbs::done();
      }
    }
  };
}

#endif
