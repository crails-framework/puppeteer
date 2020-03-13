#ifndef  DELETE_BUILD_HPP
# define DELETE_BUILD_HPP

# include "html/views/builds/destroy.hpp"
# include "app/comet/models/build.hpp"
# include "../utility/delete_view.hpp"

namespace Views
{
  class BuildDestroy : public DeleteView<HtmlTemplate::BuildDestroy, Front::Build>
  {
  };
}

#endif
