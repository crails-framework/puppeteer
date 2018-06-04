#ifndef  INSTANCE_STATE_WIDGET_HPP
# define INSTANCE_STATE_WIDGET_HPP

# include <crails/front/element.hpp>
# include "front/app/instances.hpp"
# include "app/models/instance_state.hpp"

namespace Views
{
  class InstanceStateWidget : public Crails::Front::Element
  {
    std::shared_ptr<Puppeteer::Instance> model;
    InstanceState state;
  public:
    InstanceStateWidget();
    void activate(std::shared_ptr<Puppeteer::Instance> instance);

    void fetch_state();
    void render_state();
    void render_uninstalled_state();

  private:
    void on_state_fetched(const Crails::Front::Ajax&);
    void on_state_fetch_failed(const Crails::Front::Ajax&);

    Crails::Front::Element make_configuration_state_label();
  };
}

#endif
