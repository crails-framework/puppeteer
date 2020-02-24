#ifndef  INSTANCE_ACTION_WIDGET_HPP
# define INSTANCE_ACTION_WIDGET_HPP

# include "task_runner.hpp"
# include "lib/cheerp-html/views/instance/action_widget.hpp"

class InstanceActionWidget : public HtmlTemplate::ActionWidget, public TaskRunner, public Crails::Listener
{
    bool performing_action = false;
    std::string deploying_build;
  public:
    InstanceActionWidget();

    Actions get_visible_actions();
    bool can_deploy() const;

    void set_model(std::shared_ptr<Puppeteer::Instance> instance);
    void configure();
    void uninstall();
    void deploy();

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

    void fetch_build_versions();
    std::string get_selected_build_version();
};

#endif
