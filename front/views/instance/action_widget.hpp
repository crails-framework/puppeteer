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
    Button button_deploy, button_uninstall, button_restart, button_stop;
    Crails::Front::Element progress_bar;
    bool performing_action = false;
  public:
    InstanceActionWidget();

    void activate(std::shared_ptr<Puppeteer::Instance> instance);
    void render();
    void deploy(client::Event*);
    void uninstall(client::Event*);
    void restart(client::Event*);
    void stop(client::Event*);

  private:
    const std::vector<Crails::Front::Element*> get_buttons();

    void on_performing_action();
    void on_action_performed(const Crails::Front::Ajax&);

    void on_deployed(const Crails::Front::Ajax&);
    void on_deploy_failure(const Crails::Front::Ajax&);

    void on_restarted(const Crails::Front::Ajax&);
    void on_restart_failed(const Crails::Front::Ajax&);

    void on_stopped(const Crails::Front::Ajax&);
    void on_stop_failed(const Crails::Front::Ajax&);
  };
}

#endif
