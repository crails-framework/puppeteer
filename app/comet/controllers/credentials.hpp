#ifndef  CREDENTIALS_CONTROLLER_HPP
# define CREDENTIALS_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/comet/collections/credentials.hpp"
# include "app/comet/views/credentials/index.hpp"
# include "app/comet/views/credentials/new.hpp"
# include "app/comet/views/credentials/destroy.hpp"
# include "app/comet/views/credentials/show.hpp"

struct CredentialControllerDesc
{
  static const std::string resource_path;
  typedef Front::Credentials       Collection;
  typedef Views::CredentialIndex   IndexView;
  typedef Views::CredentialShow    ShowView;
  typedef Views::CredentialNew     CreateView;
  typedef Views::CredentialNew     EditView;
  typedef Views::CredentialDestroy DestroyView;
};

typedef CrudController<CredentialControllerDesc> CredentialsController;

#endif

