#ifndef  BUILDS_COLLECTION_HPP
# define BUILDS_COLLECTION_HPP

# include "app/comet/models/build.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Builds : public Comet::ArchiveCollection<Build>
  {
  public:
    std::string get_url() const
    {
      return '/' + Build::get_scope();
    }
  };
}

#endif
