#ifndef  VIEW_BACKUP_NEW_HPP
# define VIEW_BACKUP_NEW_HPP

# include "utility/model_form.hpp"
# include "lib/cheerp-html/views/backup_new.hpp"
# include "app/backups.hpp"
# include "front/resources/elements/breadcrumbs.hpp"

namespace Views
{
  class BackupNew : public ModelForm<Puppeteer::Backup, HtmlTemplate::BackupNew>
  {
    ODB::id_type instance_id;
  public:
    BackupNew()
    {
    }

    void initialize_breadcrumbs()
    {
      if (instance_id != ODB_NULL_ID)
      {
        Breadcrumbs::reset();
        Breadcrumbs::set_instance(instance_id);
        Breadcrumbs::add_crumb("Backups", new_model()->get_path());
        Breadcrumbs::add_crumb("Edit", "#/");
        Breadcrumbs::done();
      }
    }

    inline void activate()
    {
      ModelForm::activate();
    }

    inline void activate(std::shared_ptr<Puppeteer::Backup> _model)
    {
      set_instance_id(_model->get_instance_id());
      ModelForm::activate(_model);
    }

    void update_model_attributes()
    {
      auto schedule_input    = find("[name=\"backup_schedule\"]")[0];
      auto max_history_input = find("[name=\"backup_max_history\"]")[0];

      model->set_schedule(schedule_input.get_value());
      model->set_max_history(max_history_input.value<unsigned short>());
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
