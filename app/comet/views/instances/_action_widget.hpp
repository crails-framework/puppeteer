#ifndef  INSTANCE_ACTION_WIDGET_HPP
# define INSTANCE_ACTION_WIDGET_HPP

# include "task_runner.hpp"
# include "html/views/instances/_action_widget.hpp"
# include <comet/http.hpp>

class InstanceActionWidget : public HtmlTemplate::ActionWidget, public TaskRunner, public Comet::Listener
{
    std::string deploying_build;
  public:
    InstanceActionWidget();
    virtual ~InstanceActionWidget() {}

    Actions get_visible_actions();
    bool can_deploy() const;

    void set_model(const std::shared_ptr<Front::Instance>& instance);
    void configure();
    void uninstall();
    void deploy();

  private:
    std::vector<Comet::Element> get_buttons();

    void on_remote_state_changed();
    void on_performing_action();
    void on_ajax_action_performed(const Comet::Http::Response&) { on_action_performed(); }
    void on_action_performed();

    void on_configure_start(const Comet::Http::Response&);
    void on_configure_failure(const Comet::Http::Response&);
    void on_configure_task_progress(Comet::Object);

    void on_uninstall_start(const Comet::Http::Response&);
    void on_uninstall_failure(const Comet::Http::Response&);
    void on_uninstall_task_progress(Comet::Object);

    void on_deploy_start(const Comet::Http::Response&);
    void on_deploy_failure(const Comet::Http::Response&);
    void on_deploy_task_progress(Comet::Object);

    void fetch_build_versions();
    std::string get_selected_build_version();
};

#endif
