#ifndef  BUILD_HPP
# define BUILD_HPP

# include "model.hpp"
# include "lib/app/data/build.hpp"

class OArchive;
class IArchive;

# pragma db object pointer(std::shared_ptr)
class Build : public Model, public MetaRecord::Build
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Build)
  struct Count
  {
    #pragma db column("count(" + Build::id + ")")
    size_t value;
  };

  std::string get_url()  const { return get_model_url(*this); }
  std::string get_path() const { return '#' + get_url(); }
  std::vector<std::string> get_available_builds() const;

  void serialize(OArchive&);
  void serialize(IArchive&);

# ifndef __CHEERP_CLIENT__
  static const std::string builds_path;

  bool can_destroy() const;
  void before_destroy();
  void on_change();
  bool update_last_build();
  bool update_last_build(const DataTree&);
  void remove_build(unsigned int build_id);

  void collect_variables(std::map<std::string,std::string>&);
  std::string get_build_path() const;
  std::string get_build_config();
# endif
};

#endif
