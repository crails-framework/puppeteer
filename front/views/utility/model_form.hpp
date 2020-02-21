#ifndef  MODEL_FORM_HPP
# define MODEL_FORM_HPP

# include "form_view.hpp"
# include "../../router.hpp"

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

    virtual void activate()
    {
      VIEW::trigger_binding_updates();
    }

    virtual void activate(unsigned long id)
    {
      std::cout << "Activating for id " << id << std::endl;
      fetch_one<MODEL>(id, [this](std::shared_ptr<MODEL> _model)
      {
        std::cout << "Model fetched, activating now" << std::endl;
        set_model(_model);
        std::cout << "After fetch induced activation" << std::endl;
      });
    }

    virtual void set_model(std::shared_ptr<MODEL> _model)
    {
      std::cout << "Activating view with model " << _model->get_id() << std::endl;
      model = _model;
      if (has_model())
        std::cout << "View has model" << std::endl;
      VIEW::trigger_binding_updates();
    }

    virtual void on_save_clicked()
    {
      if (!model)
        model = std::make_shared<MODEL>();
      update_model_attributes();
      model->save().then([this]() { on_saved(); });
    }

    virtual void on_saved()
    {
      Puppeteer::Router::instance->navigate(model->get_path(), true);
    }
  };
}

#endif
