#ifndef  INSTANCES_COLLECTION_HPP
# define INSTANCES_COLLECTION_HPP

# include "app/comet/models/machine.hpp"
# include "app/comet/models/instance.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Instances : public Comet::ArchiveCollection<Instance>
  {
  public:
    virtual std::string get_url() const
    {
      return '/' + Instance::get_scope();
    }
  };

  class MachineInstances : public Instances
  {
    ODB::id_type machine_id;

  public:
    void set_machine_id(ODB::id_type id) { machine_id = id; }

    std::string get_url() const
    {
      return '/' + Machine::get_scope()
           + '/' + Comet::lexical_cast<std::string>(machine_id)
           + '/' + Instance::get_scope();
    }
  };
}

#endif

