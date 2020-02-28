#ifndef  BACKUP_CONTROLLER_HPP
# define BACKUP_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/backup.hpp"

class BackupController : public Crud::ArchiveController<Backup>
{
  typedef Crud::ArchiveController<Backup> Super;
  std::string get_view_path() const { return "backup_"; }
public:
  BackupController(Crails::Params&);

  void create_or_update();
  void enable();
  void disable();
  void builds();
  void restore();
  void remove_build();

protected:
  bool find_model();
  bool initialize_model(Data);
  void after_create();
};

#endif
