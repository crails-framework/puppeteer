#ifndef  BACKUP_HPP
# define BACKUP_HPP

# include "model.hpp"
# include "lib/app/data/backup.hpp"

class OArchive;
class IArchive;

namespace Sync { class Task; }

# pragma db object pointer(std::shared_ptr)
class Backup : public Model, public ModelData::Backup
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Instance)
  struct Count
  {
    #pragma db column("count(" + Instance::id + ")")
    size_t value;
  };

  enum Action
  {
    BackupAction,
    RestoreAction
  };

  std::string get_name();

  void serialize(OArchive&);
  void serialize(IArchive&);

# ifndef __CHEERP_CLIENT__
  std::string get_build_config();
  std::string get_command() const;
  std::string get_backup_folder();
  std::string get_backup_script() const;
  std::string get_restore_script() const;
  std::string get_tarball_path_for_backup(unsigned int);
  std::string get_tarball_path_for_backup(const std::string&);

  bool can_destroy() const;
  void before_destroy();
  void on_change();
  void perform(Action, const std::string& tarball, Sync::Task&);
  void clean_up_backup_folder();
  void remove_build(unsigned int);
private:
  static const std::string base_path;
# endif
};

#endif
