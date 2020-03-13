#ifndef  BACKUPS_CONTROLLER_HPP
# define BACKUPS_CONTROLLER_HPP

# include "application.hpp"
# include "app/comet/models/backup.hpp"

class BackupsController : public ApplicationController
{
public:
  BackupsController(const Comet::Params&);

  void show();
  void update();

private:
  Comet::Promise find_model();

  unsigned long instance_id;
  std::shared_ptr<Front::Backup> model;
};

#endif
