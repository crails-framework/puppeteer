#ifndef  VIEWS_DASHBOARD_HPP
# define VIEWS_DASHBOARD_HPP

# include <crails/datatree.hpp>
# include "lib/cheerp-html/views/dashboard.hpp"

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
