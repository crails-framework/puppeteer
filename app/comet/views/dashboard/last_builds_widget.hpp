#ifndef  DASHBOARD_LAST_BUILDS_WIDGET_HPP
# define DASHBOARD_LAST_BUILDS_WIDGET_HPP

# include "html/views/dashboard/last_builds_widget.hpp"

class LastBuildsWidget : public HtmlTemplate::LastBuildsWidget
{
public:
  void fetch_last_builds();
  void aggregate_builds_from_jobs(Data);

private:
  DataTree last_builds_response;
};

#endif
