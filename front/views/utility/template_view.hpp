#ifndef  FRONT_TEMPLATE_VIEW_HPP
# define FRONT_TEMPLATE_VIEW_HPP

# include <crails/front/mvc/view.hpp>
# include <crails/front/element.hpp>
# include <crails/front/signal.hpp>
# include <iostream>

class TemplateView : public Crails::Front::View, public Crails::Listener
{
protected:
  typedef Crails::Front::Element El;
public:
  TemplateView(const std::string& name)
  {
    auto* tpl = client::document.getElementById(name.c_str());

    if (tpl)
      (*this)->set_innerHTML(tpl->get_innerHTML());
  }

  void emplace(Crails::Front::Element& target)
  {
    (*this)->set_innerHTML("");
    append_to(target);
  }

  virtual void attached() {}
  virtual void detached() {}
};

#endif
