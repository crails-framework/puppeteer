#ifndef  MODEL_VIEW_HPP
# define MODEL_VIEW_HPP

# include <memory>
# include "app/model.hpp"

namespace Views
{
  template<typename MODEL, typename VIEW>
  class ModelView : public VIEW
  {
  public:
    virtual void initialize_breadcrumbs() {}

    void activate(unsigned long model_id)
    {
      fetch_one<MODEL>(model_id, [this](std::shared_ptr<MODEL> _model)
      {
        activate(_model);
      });
    }

    void activate(std::shared_ptr<MODEL> _model)
    {
      this->model = _model;
      VIEW::wrapper.set_edit_path(this->model->get_path() + "/edit");
      VIEW::wrapper.set_destroy_path(this->model->get_path() + "/destroy");
      VIEW::wrapper.set_title(model->get_name());
      on_model_received();
      VIEW::signaler.trigger("model-changed");
      initialize_breadcrumbs();
    }

    std::shared_ptr<MODEL> get_model() const { return model; }

  protected:
    virtual void on_model_received()
    {
      VIEW::trigger_binding_updates();
    }

    std::shared_ptr<MODEL> model;
  };
}

#endif
