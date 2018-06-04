#ifndef  INSTANCE_ACTION_WIDGET_HPP
# define INSTANCE_ACTION_WIDGET_HPP

# include <crails/front/element.hpp>
# include <crails/front/signal.hpp>
# include "front/app/instances.hpp"
# include "front/views/utility/button.hpp"

namespace Views
{
  class InstanceActionWidget : public Crails::Front::Element, public Crails::Listener
  {
    std::shared_ptr<Puppeteer::Instance> model;
    Button button_deploy;
  public:
    InstanceActionWidget();

    void activate(std::shared_ptr<Puppeteer::Instance> instance);
    void deploy(client::Event*);

    void on_deployed(const Crails::Front::Ajax&);
    void on_deploy_failure(const Crails::Front::Ajax&);
  };
}

#endif
