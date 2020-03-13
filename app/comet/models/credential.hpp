#ifndef  FRONT_CREDENTIAL_HPP
# define FRONT_CREDENTIAL_HPP

# include "application.hpp"
# include "app/models/credential.hpp"

namespace Front
{
  class Credential : public ::Credential
  {
  public:
    model_class("credentials")
    model_default_constructors(Credential)
  };
}

#endif
