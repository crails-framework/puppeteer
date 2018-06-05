#ifndef  CREDENTIALS_CONTROLLER_HPP
# define CREDENTIALS_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/credential.hpp"

class CredentialController : public Crud::ArchiveController<Credential>
{
  typedef Crud::ArchiveController<Credential> Super;
  std::string get_view_path() const { return "credential_"; }
public:
  CredentialController(Crails::Params&);
};

#endif

