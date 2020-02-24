#ifndef  INSTANCE_STATE_WIDGET_HPP
# define INSTANCE_STATE_WIDGET_HPP

# include "task_runner.hpp"
# include "lib/cheerp-html/views/instance/state_widget.hpp"

class InstanceStateWidget : public HtmlTemplate::StateWidget, public TaskRunner
{
  typedef std::shared_ptr<Puppeteer::Instance> ModelPtr;
  ModelPtr model;
public:
  InstanceStateWidget();

  ModelPtr get_model() const { return model; }

  void set_model(ModelPtr value);
  void fetch_state();

  bool is_running() const { return model ? model->get_running() : false ; }
  bool is_stopped() const { return !is_running(); }
  bool is_dirty() const { return model ? model->get_state() == Instance::Dirty : false; }

  void on_start_clicked();
  void on_stop_clicked();

private:
  void on_state_fetched();

  void on_start_task_progress(Crails::Front::Object);
  void on_stop_task_progress(Crails::Front::Object);
};

#endif
