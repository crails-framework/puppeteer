#ifndef  MODEL_FORM_HPP
# define MODEL_FORM_HPP

# include "app/comet/models/application.hpp"
# include "app/comet/application.hpp"
# include <comet/globals.hpp>

namespace Views
{
  template<typename MODEL, typename VIEW>
  class ModelForm : public VIEW
  {
  protected:
    std::shared_ptr<MODEL> model;
  public:
    bool has_model() const { return model.get() != nullptr; }

    virtual void update_model_attributes() = 0;
    virtual void initialize_breadcrumbs() {};

    virtual void activate()
    {
      trigger_binding_updates();
      initialize_breadcrumbs();
    }

    virtual void activate(unsigned long id)
    {
      fetch_one<MODEL>(id, [this](std::shared_ptr<MODEL> _model)
      {
        activate(_model);
      });
    }

    virtual void activate(std::shared_ptr<MODEL> _model)
    {
      set_model(_model);
      initialize_breadcrumbs();
    }

    std::shared_ptr<MODEL> get_model() const
    {
      return model;
    }

    virtual void set_model(std::shared_ptr<MODEL> _model)
    {
      model = _model;
      Comet::window->setTimeout(cheerp::Callback([this]()
      {
        trigger_binding_updates();
      }), 500);
    }

    virtual std::shared_ptr<MODEL> new_model()
    {
      return std::make_shared<MODEL>();
    }

    virtual void on_save_clicked()
    {
      if (!model)
        model = new_model();
      update_model_attributes();
      model->save().then([this]() { on_saved(); });
    }

    virtual void on_saved()
    {
      Application::get().router.navigate(model->get_path(), true);
    }

    virtual void trigger_binding_updates()
    {
      VIEW::trigger_binding_updates();
    }
  };
}

#endif
