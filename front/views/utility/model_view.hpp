#ifndef  MODEL_VIEW_HPP
# define MODEL_VIEW_HPP

# include "template_view.hpp"
# include "theme.hpp"

namespace Views
{
  template<typename MODEL>
  class ModelView : public TemplateView
  {
  public:
    ModelView(const std::string& type_title) : TemplateView("tpl1")
    {
      edit_link = El("a", {{"class","au-btn au-btn-icon au-btn--blue"}}).inner({
        Theme::fa_icon("edit"),
        El("span").text("Edit")	
      });
      title_controls.inner({edit_link});

      title = El("h3");
      el.add_class("container-fluid");
      el.inner({
        Theme::title1(El("div").inner({
          El("h2", {{"class","title-1"}}).text(type_title),
          title
        }), title_controls),
	page_content
      });
    }

    void activate(unsigned long model_id)
    {
      fetch_one<MODEL>(model_id, [this](std::shared_ptr<MODEL> _model)
      {
        this->model = _model;
        this->edit_link.attr("href", this->model->get_path() + "/edit");
        title.text(model->get_name());
        on_model_received();
      });
    }

  protected:
    virtual void on_model_received() {}

    std::shared_ptr<MODEL> model;
    Crails::Front::Element title, title_controls, page_content;
    Crails::Front::Element edit_link;
  };
}

#endif
