#ifndef  INSTANCE_HPP
# define INSTANCE_HPP

# include "modules/odb/model.hpp"
# include "lib/app/data/instance.hpp"

class OArchive;
class IArchive;

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

  void serialize(OArchive&);
  void serialize(IArchive&);

  void install();
};

#endif
