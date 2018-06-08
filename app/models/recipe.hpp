#ifndef  RECIPE_HPP
# define RECIPE_HPP

# include "modules/odb/model.hpp"
# include "lib/app/data/recipe.hpp"

class OArchive;
class IArchive;
class Instance;
namespace Sync { class Task; }

# pragma db object pointer(std::shared_ptr)
class Recipe : public Db::Model, public ModelData::Recipe
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Recipe)
  struct Count
  {
    #pragma db column("count(" + Recipe::id + ")")
    size_t value;
  };

  static const std::string base_path, remote_path, remote_user;

  void serialize(OArchive&);
  void serialize(IArchive&);

  void on_change();

  std::string get_path() const;
  void fetch_recipe();

  void exec_package(const std::string&, Instance&, Sync::Task&);
  void deploy_for(Instance&, Sync::Task&);
  void uninstall_from(Instance&, Sync::Task&);
};

#endif
