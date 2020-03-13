#ifndef  DELETE_MACHINE_HPP
# define DELETE_MACHINE_HPP

# include "html/views/machines/destroy.hpp"
# include "app/comet/collections/machines.hpp"
# include "../utility/delete_view.hpp"

namespace Views
{
  class MachineDestroy : public DeleteView<HtmlTemplate::MachineDestroy, Front::Machine>
  {
  };
}

#endif
