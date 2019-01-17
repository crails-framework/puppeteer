#ifndef  MODEL_FORM_HPP
# define MODEL_FORM_HPP

# include "template_view.hpp"
# include "form_view.hpp"
# include "../../router.hpp"

namespace Views
{
  template<typename MODEL>
  class ModelForm : public TemplateView
  {
  protected:
    FormView               form_view;
    std::shared_ptr<MODEL> model;
  public:
    ModelForm(const std::string& title) : TemplateView("tpl1"), form_view(title)
    {
      listen_to(form_view.sent, std::bind(&ModelForm<MODEL>::save_clicked, this, std::placeholders::_1));
    }

    virtual std::unordered_map<std::string, El> get_inputs() = 0;
    virtual void update_model_attributes() = 0; 
    virtual void update_form_attributes() = 0;

    void initialize_form()
    {
      Crails::Front::Element col_lg_12;

      col_lg_12.add_class("col-lg-12");
      inner({
        El("div", {{"class","container-fluid"}}).inner({
          El("div", {{"class", "row"}}).inner({
            col_lg_12
          })
        })
      });
      form_view.set_inputs(get_inputs());
      form_view.append_to(col_lg_12);
      form_view.initialize_form();
    }

    void attached()
    {
      initialize_form();
      form_view.attached();
    }

    void detached()
    {
      form_view.detached();
    }

    void activate()
    {
      model = std::make_shared<MODEL>();
      update_form_attributes();
    }

    void activate(unsigned long id)
    {
      fetch_one<MODEL>(id, [this](std::shared_ptr<MODEL> _model)
      {
        activate(_model);
      });
    }

    void activate(std::shared_ptr<MODEL> _model)
    {
      model = _model;
      update_form_attributes();
    }

    unsigned long get_my_id() { return 42; }

    void save_clicked(client::Event*)
    {
      update_model_attributes();
      model->save().then([this]() { on_saved(); });
    }

    virtual void on_saved()
    {
	    std::cout << "Gonna navigate to path " << model->get_path() << std::endl;
      Puppeteer::Router::instance->navigate(model->get_path(), true);
    }

    virtual void on_save_failed()
    {
    }
  };
}

#endif
