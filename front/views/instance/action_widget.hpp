#ifndef  INSTANCE_ACTION_WIDGET_HPP
# define INSTANCE_ACTION_WIDGET_HPP

# include <crails/front/element.hpp>
# include <crails/front/signal.hpp>
# include "front/app/instances.hpp"
# include "front/views/utility/button.hpp"
# include "front/views/utility/progress_bar.hpp"
# include "console_output.hpp"

namespace Sync
{
  enum TaskState
  {
    Continue = 0,
    Success  = 1,
    Abort    = 2
  };
}

namespace Views
{
  class InstanceActionWidget : public Crails::Front::Element, public Crails::Listener
  {
    std::shared_ptr<Puppeteer::Instance> model;
    Button button_deploy, button_uninstall, button_restart, button_stop;
    ProgressBar     progress_bar;
    ConsoleOutput&  console_output;
    bool performing_action = false;
  public:
    InstanceActionWidget(ConsoleOutput& console_output);

    void activate(std::shared_ptr<Puppeteer::Instance> instance);
    void render();
    void deploy(client::Event*);
    void uninstall(client::Event*);
    void restart(client::Event*);
    void stop(client::Event*);

  private:
    const std::vector<Crails::Front::Element*> get_buttons();

    void on_performing_action();
    void on_ajax_action_performed(const Crails::Front::Ajax&) { on_action_performed(); }
    void on_action_performed();

    void on_deploy_start(const Crails::Front::Ajax&);
    void on_deploy_failure(const Crails::Front::Ajax&);
    void on_deploy_task_progress(Crails::Front::Object);

    void on_uninstall_start(const Crails::Front::Ajax&);
    void on_uninstall_failure(const Crails::Front::Ajax&);
    void on_uninstall_task_progress(Crails::Front::Object);

    Sync::TaskState on_task_progress(Crails::Front::Object);

    void on_restarted(const Crails::Front::Ajax&);
    void on_restart_failed(const Crails::Front::Ajax&);

    void on_stopped(const Crails::Front::Ajax&);
    void on_stop_failed(const Crails::Front::Ajax&);
  };
}

#endif
