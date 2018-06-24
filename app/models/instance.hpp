#ifndef  INSTANCE_HPP
# define INSTANCE_HPP

# include "model.hpp"
# include "lib/app/data/instance.hpp"

class OArchive;
class IArchive;
namespace Ssh { class Session; }
namespace Sync { class Task; }

# pragma db object pointer(std::shared_ptr)
class Instance : public Model, public ModelData::Instance
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
    Dirty       = 2
  };

  void serialize(OArchive&);
  void serialize(IArchive&);

  void collect_variables(std::map<std::string,std::string>&) const;
  void configure(Sync::Task&);
  void uninstall(Sync::Task&);
  void open_ssh(std::function<void (Ssh::Session&)>);

  bool needs_restart();
  bool needs_configure();
};

#endif
