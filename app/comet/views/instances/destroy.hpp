#ifndef  DELETE_INSTANCE_HPP
# define DELETE_INSTANCE_HPP

# include "html/views/instances/destroy.hpp"
# include "app/comet/models/instance.hpp"
# include "../utility/delete_view.hpp"

namespace Views
{
  class InstanceDestroy : public DeleteView<HtmlTemplate::InstanceDestroy, Front::Instance>
  {
  };
}

#endif
