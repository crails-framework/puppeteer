#ifndef  BUILD_HPP
# define BUILD_HPP

# include "modules/odb/model.hpp"
# include "lib/app/data/build.hpp"

class OArchive;
class IArchive;

# pragma db object pointer(std::shared_ptr)
class Build : public Db::Model, public ModelData::Build
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Build)
  struct Count
  {
    #pragma db column("count(" + Build::id + ")")
    size_t value;
  };

  static const std::string builds_path;

  void serialize(OArchive&);
  void serialize(IArchive&);

  void on_change();
  void update_last_build();
  void update_last_build(const DataTree&);

  void collect_variables(std::map<std::string,std::string>&);
  std::string get_build_path() const;
  std::string get_build_config();
};

#endif
