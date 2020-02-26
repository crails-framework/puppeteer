#ifndef  DELETE_BUILD_HPP
# define DELETE_BUILD_HPP

# include "lib/cheerp-html/views/delete/delete_build.hpp"
# include "app/builds.hpp"
# include "view.hpp"

namespace Views
{
  class DeleteBuild : public DeleteView<HtmlTemplate::DeleteBuild, Puppeteer::Build>
  {
  };
}

#endif
