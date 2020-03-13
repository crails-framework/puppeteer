#ifndef  DELETE_CREDENTIAL_HPP
# define DELETE_CREDENTIAL_HPP

# include "html/views/credentials/destroy.hpp"
# include "app/comet/models/credential.hpp"
# include "../utility/delete_view.hpp"

namespace Views
{
  class CredentialDestroy : public DeleteView<HtmlTemplate::CredentialDestroy, Front::Credential>
  {
  };
}

#endif
