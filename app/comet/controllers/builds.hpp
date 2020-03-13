#ifndef  BUILDS_CONTROLLER_HPP
# define BUILDS_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/comet/collections/builds.hpp"
# include "app/comet/views/builds/index.hpp"
# include "app/comet/views/builds/new.hpp"
# include "app/comet/views/builds/destroy.hpp"
# include "app/comet/views/builds/show.hpp"

struct BuildControllerDesc
{
  static const std::string resource_path;
  typedef Front::Builds       Collection;
  typedef Views::BuildIndex   IndexView;
  typedef Views::BuildShow    ShowView;
  typedef Views::BuildNew     CreateView;
  typedef Views::BuildNew     EditView;
  typedef Views::BuildDestroy DestroyView;
};

typedef CrudController<BuildControllerDesc> BuildsController;

#endif
