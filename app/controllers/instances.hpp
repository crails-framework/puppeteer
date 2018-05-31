#ifndef  INSTANCES_CONTROLLER_HPP
# define INSTANCES_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/instance.hpp"

class InstanceController : public Crud::ArchiveController<Instance>
{
  typedef Crud::ArchiveController<Instance> Super;
  std::string get_view_path() const { return "instance_"; }
public:
  InstanceController(Crails::Params&);

  void configure();
};

#endif

