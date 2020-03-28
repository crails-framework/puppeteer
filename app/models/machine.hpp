#ifndef  MACHINE_HPP
# define MACHINE_HPP

# include "model.hpp"
# include "lib/app/data/machine.hpp"

class OArchive;
class IArchive;
namespace Ssh { class Session; }

# pragma db object pointer(std::shared_ptr)
class Machine : public Model, public ModelData::Machine
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Machine)
  struct Count
  {
    #pragma db column("count(" + Machine::id + ")")
    size_t value;
  };

  std::string get_url()  const { return get_model_url(*this); }
  std::string get_path() const { return '#' + get_url(); }

  void serialize(OArchive&);
  void serialize(IArchive&);

#ifndef __CHEERP_CLIENT__
  void before_destroy();
  bool can_destroy() const;

  void open_ssh(std::function<void (Ssh::Session&)> callback) const;
#endif
};

#endif
