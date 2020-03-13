#ifndef  FRONT_BACKUP_HPP
# define FRONT_BACKUP_HPP

# include "application.hpp"
# include "app/models/backup.hpp"

namespace Front
{
  class Backup : public ::Backup
  {
  public:
    model_default_constructors(Backup)

    static std::string get_scope() { return "backup"; }

    std::string get_url() const
    {
      return get_url_for_instance(get_instance_id());
    }

    std::string get_path() const
    {
      return get_path_for_instance(get_instance_id());
    }

    std::string get_name() const
    {
      return "Backups";
    }

    static std::string get_url_for_instance(ODB::id_type id);
    static std::string get_path_for_instance(ODB::id_type id);
    static void fetch_for_instance(ODB::id_type id, std::function<void (std::shared_ptr<Backup>)> callback);
  };
}

#endif

