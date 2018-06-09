#ifndef  VIEW_PROGRESS_BAR_HPP
# define VIEW_PROGRESS_BAR_HPP

# include <crails/front/element.hpp>
# include <iostream>

namespace Views
{
  class ProgressBar : public Crails::Front::Element
  {
  public:
    ProgressBar()
    {
      attr("class", "progress-bar bg-info progress-bar-striped progress-bar-animated");
      attr("style", "width:0%");
    }

    void set_active(bool active)
    {
      toggle_class("progress-bar-animated", active);
    }

    void set_progress(float value)
    {
      std::stringstream stream;

      stream << "width:" << (value * 100) << '%';
      std::cout << "progress bar style: " << stream.str() << std::endl;
      attr("style", stream.str());
    }
  };
}

#endif
