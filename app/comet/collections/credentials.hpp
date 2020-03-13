#ifndef  CREDENTIALS_COLLECTION_HPP
# define CREDENTIALS_COLLECTION_HPP

# include "app/comet/models/credential.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Credentials : public Comet::ArchiveCollection<Credential>
  {
  public:
    std::string get_url() const
    {
      return '/' + Credential::get_scope();
    }
  };
}

#endif
