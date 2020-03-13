#ifndef  INSTANCES_CONTROLLER_HPP
# define INSTANCES_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/comet/collections/instances.hpp"
# include "app/comet/views/instances/index.hpp"
# include "app/comet/views/instances/new.hpp"
# include "app/comet/views/instances/destroy.hpp"
# include "app/comet/views/instances/show.hpp"

struct InstanceControllerDesc
{
  static const std::string resource_path;
  typedef Front::Instances       Collection;
  typedef Views::InstanceIndex   IndexView;
  typedef Views::InstanceShow    ShowView;
  typedef Views::InstanceNew     CreateView;
  typedef Views::InstanceNew     EditView;
  typedef Views::InstanceDestroy DestroyView;
};

typedef CrudController<InstanceControllerDesc> InstancesController;

#endif
