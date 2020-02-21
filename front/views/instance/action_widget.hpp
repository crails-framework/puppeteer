#ifndef  INSTANCE_ACTION_WIDGET_HPP
# define INSTANCE_ACTION_WIDGET_HPP

# include "console_output.hpp"
# include "lib/cheerp-html/views/instance/action_widget.hpp"

namespace Sync
{
  enum TaskState
  {
    Continue = 0,
    Success  = 1,
    Abort    = 2
  };
}

class InstanceActionWidget : public HtmlTemplate::ActionWidget, public Crails::Listener
{
    ConsoleOutput* console_output = nullptr;
    bool performing_action = false;
  public:
    InstanceActionWidget();

    Actions get_visible_actions();

    void set_console_output(ConsoleOutput* value) { console_output = value; }
    void set_model(std::shared_ptr<Puppeteer::Instance> instance);
    void configure();
    void uninstall();
    void deploy();
    void restart();
    void stop();

  private:
    std::vector<Crails::Front::Element> get_buttons();

    void on_remote_state_changed();
    void on_performing_action();
    void on_ajax_action_performed(const Crails::Front::Ajax&) { on_action_performed(); }
    void on_action_performed();

    void on_configure_start(const Crails::Front::Ajax&);
    void on_configure_failure(const Crails::Front::Ajax&);
    void on_configure_task_progress(Crails::Front::Object);

    void on_uninstall_start(const Crails::Front::Ajax&);
    void on_uninstall_failure(const Crails::Front::Ajax&);
    void on_uninstall_task_progress(Crails::Front::Object);

    void on_deploy_start(const Crails::Front::Ajax&);
    void on_deploy_failure(const Crails::Front::Ajax&);
    void on_deploy_task_progress(Crails::Front::Object);

    Sync::TaskState on_task_progress(Crails::Front::Object);

    void on_restarted(const Crails::Front::Ajax&);
    void on_restart_failed(const Crails::Front::Ajax&);

    void on_stopped(const Crails::Front::Ajax&);
    void on_stop_failed(const Crails::Front::Ajax&);
};

#endif
