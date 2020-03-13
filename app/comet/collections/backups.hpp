#ifndef  BACKUPS_COLLECTION_HPP
# define BACKUPS_COLLECTION_HPP

# include "app/comet/models/backup.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Backups : public Comet::ArchiveCollection<Backup>
  {
  public:
    std::string get_url() const
    {
      return '/' + Backup::get_scope();
    }
  };
}

#endif

