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

  void by_machine();
  void configure();
  void uninstall();
  void deploy();
  void start();
  void stop();

  void fetch_state();
private:
  void schedule_task(const std::string& task_name, Data task_params);
  void on_task_started(const std::string& task_name, const std::string& task_uid);
  void open_ssh(std::function<void(Ssh::Session&)>);
};

#endif

