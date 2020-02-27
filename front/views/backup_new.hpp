#ifndef  VIEW_BACKUP_NEW_HPP
# define VIEW_BACKUP_NEW_HPP

# include "utility/model_form.hpp"
# include "lib/cheerp-html/views/backup_new.hpp"
# include "app/backups.hpp"

namespace Views
{
  class BackupNew : public ModelForm<Puppeteer::Backup, HtmlTemplate::BackupNew>
  {
    ODB::id_type instance_id;
  public:
    BackupNew()
    {
    }

    void update_model_attributes()
    {
      auto schedule_input = find("[name=\"backup_schedule\"]")[0];

      model->set_schedule(schedule_input.get_value());
    }

    void set_instance_id(ODB::id_type value) { instance_id = value; }

    std::shared_ptr<Puppeteer::Backup> new_model()
    {
      auto ptr = std::make_shared<Puppeteer::Backup>();

      ptr->set_instance_id(instance_id);
      return ptr;
    }
  };
}

#endif
