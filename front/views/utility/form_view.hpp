#ifndef  FORM_VIEW_HPP
# define FORM_VIEW_HPP

# include "template_view.hpp"
# include "theme.hpp"
# include <crails/front/signal.hpp>
# include <unordered_map>

namespace Views
{
  class FormView : public TemplateView
  {
  public:
    Crails::Signal<client::Event*> sent;

    FormView(const std::string& title) : TemplateView("tpl1")
    {
      std::cout << "FormView constructor" << std::endl;
      Theme::_card(*this, title, form_el);
      button_save = El("button", {{"class","btn btn-lg btn-info btn-block"}}).inner({
        Theme::fa_icon("save"),
        El("span").text(" Save")
      });
    }

    ~FormView()
    {
      std::cout << "FormView destructor" << std::endl;
    }

    void initialize_form()
    {
      std::vector<Crails::Front::Element> elements;

      std::for_each(inputs.begin(), inputs.end(), [&elements](std::pair<std::string, El> entry)
      {
        elements.push_back(Theme::form_group(entry.first, entry.second));
      });
      elements.push_back(El("div").inner({ button_save }));
      form_el.html("").inner(elements);
    }

    void set_inputs(std::unordered_map<std::string, Crails::Front::Element> value)
    {
      inputs = value;
      initialize_form();
    }

    void attached()
    {
      button_listener = cheerp::Callback([this](client::Event* event) {
        std::cout << "button_listener triggered" << std::endl;
        sent.trigger(event);
        std::cout << "button_listener triggered /end" << std::endl;
      });
      button_save->addEventListener("click", button_listener);
    }

    void detached()
    {
      button_save->removeEventListener("click", button_listener);
    }

  private:
    Crails::Front::Element form_el, button_save;
    std::unordered_map<std::string, Crails::Front::Element> inputs;
    client::EventListener* button_listener;
  };
}

#endif
