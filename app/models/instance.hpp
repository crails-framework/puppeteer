#ifndef  INSTANCE_HPP
# define INSTANCE_HPP

# include "model.hpp"
# include "lib/app/data/instance.hpp"

class OArchive;
class IArchive;
namespace Ssh { class Session; }
namespace Sync { class Task; }

# pragma db object pointer(std::shared_ptr)
class Instance : public Model, public MetaRecord::Instance
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Instance)
  struct Count
  {
    #pragma db column("count(" + Instance::id + ")")
    size_t value;
  };

  enum State
  {
    Uninstalled = 0,
    Ready       = 1,
    Dirty       = 2,
    Deployed    = 3
  };

  std::string get_url()  const { return get_model_url(*this); }
  std::string get_path() const { return '#' + get_url(); }
  bool can_start_task() const { return running_task.length() == 0; }

  void start(Sync::Task&);
  void stop(Sync::Task&);

  void serialize(OArchive&);
  void serialize(IArchive&);

  std::shared_ptr<Recipe> get_instance_recipe();

# ifndef __CHEERP_CLIENT__
  void collect_variables(std::map<std::string,std::string>&);
  void configure(Sync::Task&);
  void uninstall(Sync::Task&);
  void deploy(Sync::Task&, const std::string& build_id);
  void update_running_state(Sync::Task&);
  void open_ssh(std::function<void (Ssh::Session&)>);

  bool needs_restart();
  bool needs_configure();

  bool can_destroy() const;
  void before_destroy();
#endif
};

#endif
