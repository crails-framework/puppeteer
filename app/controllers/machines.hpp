#ifndef  MACHINES_CONTROLLER_HPP
# define MACHINES_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/machine.hpp"

class MachineController : public Crud::ArchiveController<Machine>
{
  typedef Crud::ArchiveController<Machine> Super;
  std::string get_view_path() const { return "machine_"; }
public:
  MachineController(Crails::Params&);
};

#endif
