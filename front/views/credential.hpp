#ifndef  VIEW_CREDENTIAL_HPP
# define VIEW_CREDENTIAL_HPP

# include "utility/model_view.hpp"
# include "../app/credentials.hpp"

namespace Views
{
  class Credential : public ModelView<Puppeteer::Credential>
  {
  public:
    Credential() : ModelView("Credentials")
    {
    }
  };
}

#endif
