#ifndef  MACHINES_CONTROLLER_HPP
# define MACHINES_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/comet/collections/machines.hpp"
# include "app/comet/views/machines/index.hpp"
# include "app/comet/views/machines/new.hpp"
# include "app/comet/views/machines/destroy.hpp"
# include "app/comet/views/machines/show.hpp"

struct MachineControllerDesc
{
  static const std::string resource_path;
  typedef Front::Machines       Collection;
  typedef Views::MachineIndex   IndexView;
  typedef Views::MachineShow    ShowView;
  typedef Views::MachineNew     CreateView;
  typedef Views::MachineNew     EditView;
  typedef Views::MachineDestroy DestroyView;
};

typedef CrudController<MachineControllerDesc> MachinesController;

#endif
