#ifndef  VARIABLE_SET_HPP
# define VARIABLE_SET_HPP

# include "model.hpp"
# include "lib/app/data/variable_set.hpp"

class OArchive;
class IArchive;

# pragma db object pointer(std::shared_ptr)
class VariableSet : public Model, public ModelData::VariableSet
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(VariableSet)
  struct Count
  {
    #pragma db column("count(" + VariableSet::id + ")")
    size_t value;
  };

  void serialize(OArchive&);
  void serialize(IArchive&);

  void collect_variables(std::map<std::string,std::string>& variables) const;
  static void collect_global_variables(std::map<std::string,std::string>& variables);
};

#endif
