#ifndef  FRONT_CREDENTIALS_HPP
# define FRONT_CREDENTIALS_HPP

# include "model.hpp"
# include "data/credential.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Credential : public CredentialData
  {
  public:
    model_class("credentials")
    model_default_constructors(Credential)
  };

  class Credentials : public Crails::Front::Collection<Credential>
  {
  public:
    std::string get_url() const
    {
      return '/' + Credential::get_scope();
    }
  };
}

#endif

