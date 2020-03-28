#ifndef  RECIPE_HPP
# define RECIPE_HPP

# include "model.hpp"
# include "lib/app/data/recipe.hpp"

class OArchive;
class IArchive;
class Instance;
namespace Sync { class Task; class Stream; }

# pragma db object pointer(std::shared_ptr)
class Recipe : public Model, public MetaRecord::Recipe
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Recipe)
  struct Count
  {
    #pragma db column("count(" + Recipe::id + ")")
    size_t value;
  };

  std::string get_url()  const { return get_model_url(*this); }
  std::string get_path() const { return '#' + get_url(); }

  void serialize(OArchive&);
  void serialize(IArchive&);

# ifndef __CHEERP_CLIENT__
  struct Plugin
  {
    virtual bool recipe_uses_plugin(const std::string& recipe_folder) const = 0;
    virtual void apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const std::map<std::string, std::string>& variables, Sync::Stream&) const = 0;
  };

  static const std::string base_path, remote_path, remote_user;
  static const std::vector<std::shared_ptr<Plugin> > plugins;

  bool can_destroy() const;
  void before_destroy();
  void on_change();

  std::string get_repository_path() const;
  void fetch_recipe();

  void exec_package(const std::string&, Instance&, Sync::Task&);
  void exec_script(const std::string&, Instance&, Sync::Task&);
  void deploy_for(Instance&, Sync::Task&);
  void uninstall_from(Instance&, Sync::Task&);
  void deploy_build_for(Instance&, Sync::Task&, const std::string& build_id);
# endif
};

#endif
