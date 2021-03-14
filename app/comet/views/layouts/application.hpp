#ifndef  APPLICATION_LAYOUT_HPP
# define APPLICATION_LAYOUT_HPP

# include "html/views/layouts/application.hpp"

class ApplicationLayout : public HtmlTemplate::Application
{
public:
  ApplicationLayout();
  virtual ~ApplicationLayout();

  inline void reset_search_input()
  {
    search_input.reset();
  }
};

#endif

