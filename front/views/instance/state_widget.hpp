#ifndef  INSTANCE_STATE_WIDGET_HPP
# define INSTANCE_STATE_WIDGET_HPP

# include <crails/front/element.hpp>
# include <crails/front/signal.hpp>
# include "front/app/instances.hpp"
# include "../utility/button.hpp"

namespace Views
{
  class InstanceStateWidget : public Crails::Front::Element, Crails::Listener
  {
    std::shared_ptr<Puppeteer::Instance> model;
    Button fetch_state_button;
    bool fetching_state = false;
  public:
    InstanceStateWidget();
    void activate(std::shared_ptr<Puppeteer::Instance> instance);

    void fetch_state();
    void render();
    void render_state();
    void render_uninstalled_state();

  private:
    void on_fetch_state_clicked(client::Event*);
    void on_state_fetched(const Crails::Front::Ajax&);
    void on_state_fetch_failed(const Crails::Front::Ajax&);

    Crails::Front::Element make_configuration_state_label();
    Crails::Front::Element make_status_label();
  };
}

#endif
