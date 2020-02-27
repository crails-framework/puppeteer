#ifndef  FRONT_BACKUPS_HPP
# define FRONT_BACKUPS_HPP

# include "app/models/backup.hpp"
# include "model.hpp"
# include <crails/front/mvc/collection.hpp>

namespace Puppeteer
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

    static std::string get_url_for_instance(ODB::id_type id);
    static std::string get_path_for_instance(ODB::id_type id);
    static void fetch_for_instance(ODB::id_type id, std::function<void (std::shared_ptr<Backup>)> callback);
  };

  class Backups : public Crails::Front::Collection<Backup>
  {
  public:
    std::string get_url() const
    {
      return '/' + Backup::get_scope();
    }
  };
}

#endif
