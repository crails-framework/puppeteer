#ifndef  DELETE_MACHINE_HPP
# define DELETE_MACHINE_HPP

# include "lib/cheerp-html/views/delete/delete_machine.hpp"
# include "app/machines.hpp"
# include "view.hpp"

namespace Views
{
  class DeleteMachine : public DeleteView<HtmlTemplate::DeleteMachine, Puppeteer::Machine>
  {
  };
}

#endif
