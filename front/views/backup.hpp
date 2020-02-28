#ifndef  VIEWS_BACKUP_HPP
# define VIEWS_BACKUP_HPP

# include "utility/model_view.hpp"
# include "lib/cheerp-html/views/backup.hpp"
# include "../app/backups.hpp"

namespace Views
{
  class Backup : public ModelView<Puppeteer::Backup, HtmlTemplate::Backup>
  {
  public:
    void on_model_received()
    {
      refresh_builds();
    }

    std::vector<Crails::Front::Object>& get_builds() { return builds; }
    void set_enabled_state(bool value);

    void refresh_builds();
    void on_enabled_clicked(bool value);
    void on_restore_build_clicked(std::string);
    void on_remove_build_clicked(std::string);

    std::string get_date_string(Crails::Front::Object) const;

  private:
    std::vector<Crails::Front::Object> builds;
  };
}

#endif
