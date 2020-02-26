#ifndef  DELETE_INSTANCE_HPP
# define DELETE_INSTANCE_HPP

# include "lib/cheerp-html/views/delete/delete_instance.hpp"
# include "app/instances.hpp"
# include "view.hpp"

namespace Views
{
  class DeleteInstance : public DeleteView<HtmlTemplate::DeleteInstance, Puppeteer::Instance>
  {
  };
}

#endif
