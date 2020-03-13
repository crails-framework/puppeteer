#ifndef  MACHINES_COLLECTION_HPP
# define MACHINES_COLLECTION_HPP

# include "app/comet/models/machine.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Machines : public Comet::ArchiveCollection<Machine>
  {
  public:
    std::string get_url() const
    {
      return '/' + Machine::get_scope();
    }
  };
}

#endif
