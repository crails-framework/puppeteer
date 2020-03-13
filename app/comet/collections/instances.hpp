#ifndef  INSTANCES_COLLECTION_HPP
# define INSTANCES_COLLECTION_HPP

# include "app/comet/models/instance.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Instances : public Comet::ArchiveCollection<Instance>
  {
  public:
    std::string get_url() const
    {
      return '/' + Instance::get_scope();
    }
  };
}

#endif

