#ifndef  FRONT_CREDENTIALS_HPP
# define FRONT_CREDENTIALS_HPP

# include "model.hpp"
# include "app/models/credential.hpp"
# include <crails/front/mvc/collection.hpp>
# include <sstream>

namespace Puppeteer
{
  class Credential : public ::Credential
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

