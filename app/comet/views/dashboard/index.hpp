#ifndef  VIEWS_DASHBOARD_HPP
# define VIEWS_DASHBOARD_HPP

# include <crails/datatree.hpp>
# include "html/views/dashboard/index.hpp"

namespace Views
{
  class Dashboard : public HtmlTemplate::Dashboard
  {
  public:
    void initialize_breadcrumbs();
    void activate();
  };
}

#endif
