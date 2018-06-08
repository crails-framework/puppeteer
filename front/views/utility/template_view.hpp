#ifndef  FRONT_TEMPLATE_VIEW_HPP
# define FRONT_TEMPLATE_VIEW_HPP

# include <crails/front/mvc/view.hpp>
# include <crails/front/element.hpp>
# include <iostream>

class TemplateView : public Crails::Front::View
{
public:
  TemplateView(const std::string& name)
  {
    auto* tpl = client::document.getElementById(name.c_str());

    if (tpl)
      el->set_innerHTML(tpl->get_innerHTML());
  }

  void append_to(Crails::Front::Element& target)
  {
    el.append_to(*target);
  }

  void emplace(Crails::Front::Element& target)
  {
    target->set_innerHTML("");
    append_to(target);
  }

  virtual void attached() {}
  virtual void detached() {}
};

#endif
