#ifndef  VIEW_CREDENTIAL_HPP
# define VIEW_CREDENTIAL_HPP

# include "utility/model_view.hpp"
# include "../app/credentials.hpp"

# include "lib/cheerp-html/views/credential.hpp"

namespace Views
{
  class Credential : public ModelView<Puppeteer::Credential, HtmlTemplate::Credential>
  {
  };
}

#endif
