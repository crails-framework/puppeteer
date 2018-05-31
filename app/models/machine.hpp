#ifndef  MACHINE_HPP
# define MACHINE_HPP

# include "modules/odb/model.hpp"
# include "lib/app/data/machine.hpp"

class OArchive;
class IArchive;

# pragma db object pointer(std::shared_ptr)
class Machine : public Db::Model, public ModelData::Machine
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Machine)
  struct Count
  {
    #pragma db column("count(" + Machine::id + ")")
    size_t value;
  };

  void serialize(OArchive&);
  void serialize(IArchive&);
};

#endif
