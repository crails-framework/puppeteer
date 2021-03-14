#ifndef  INSTANCE_STATE_WIDGET_HPP
# define INSTANCE_STATE_WIDGET_HPP

# include "task_runner.hpp"
# include "html/views/instances/_state_widget.hpp"

class InstanceStateWidget : public HtmlTemplate::StateWidget, public TaskRunner
{
  typedef std::shared_ptr<Front::Instance> ModelPtr;
  ModelPtr model;
  bool state_fetched = false;
public:
  InstanceStateWidget();
  virtual ~InstanceStateWidget() {}

  ModelPtr get_model() const { return model; }

  void set_model(ModelPtr value);
  void fetch_state();

  bool is_running() const { return state_fetched ? model->get_running() : false ; }
  bool is_stopped() const { return state_fetched ? !is_running() : false; }
  bool is_dirty()   const { return state_fetched ? model->get_state() == Instance::Dirty : false; }

  void on_start_clicked();
  void on_stop_clicked();

private:
  void on_state_fetched();

  void on_start_task_progress(Comet::Object);
  void on_stop_task_progress(Comet::Object);
};

#endif
