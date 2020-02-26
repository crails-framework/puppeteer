#ifndef  DELETE_CREDENTIAL_HPP
# define DELETE_CREDENTIAL_HPP

# include "lib/cheerp-html/views/delete/delete_credential.hpp"
# include "app/credentials.hpp"
# include "view.hpp"

namespace Views
{
  class DeleteCredential : public DeleteView<HtmlTemplate::DeleteCredential, Puppeteer::Credential>
  {
  };
}

#endif
