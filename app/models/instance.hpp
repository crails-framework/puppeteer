#ifndef  INSTANCE_HPP
# define INSTANCE_HPP

# include "modules/odb/model.hpp"
# include "lib/app/data/instance.hpp"

class OArchive;
class IArchive;
namespace Ssh { class Session; }

# pragma db object pointer(std::shared_ptr)
class Instance : public Db::Model, public ModelData::Instance
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
  void configure();
  void uninstall();
  void open_ssh(std::function<void (Ssh::Session&)>);

  bool needs_restart();
  bool needs_configure();
};

#endif
