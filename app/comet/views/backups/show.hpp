#ifndef  VIEWS_BACKUP_HPP
# define VIEWS_BACKUP_HPP

# include "../utility/model_view.hpp"
# include "html/views/backups/show.hpp"
# include "app/comet/models/backup.hpp"

namespace Views
{
  class BackupShow : public ModelView<Front::Backup, HtmlTemplate::BackupShow>
  {
  public:
    void initialize_breadcrumbs();

    void on_model_received()
    {
      refresh_builds();
    }

    std::vector<Comet::Object>& get_builds() { return builds; }
    void set_enabled_state(bool value);

    void refresh_builds();
    void on_enabled_clicked(bool value);
    void on_restore_build_clicked(std::string);
    void on_remove_build_clicked(std::string);

    std::string get_backup_date_string(Comet::Object) const;

  private:
    std::vector<Comet::Object> builds;
  };
}

#endif
