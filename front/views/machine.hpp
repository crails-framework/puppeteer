#ifndef  VIEW_MACHINE_HPP
# define VIEW_MACHINE_HPP

# include "utility/template_view.hpp"
# include "utility/theme.hpp"
# include <iostream>
# include "../app/machines.hpp"

namespace Views
{
  class Machine : public TemplateView
  {
  public:
    Machine() : TemplateView("tpl1")
    {
      typedef Crails::Front::Element El;

      title = El("h3");
      el.add_class("container-fluid");
      el.inner({
        Theme::title1(El("div").inner({
          El("h2", {{"class","title-1"}}).text("Machine"),
          title
        }), El())
      });
    }

    void activate(unsigned long machine_id)
    {
      fetch_one<Puppeteer::Machine>(machine_id, [this](std::shared_ptr<Puppeteer::Machine> machine)
      {
        std::cout << "Fetched machine, finish activation" << std::endl;
        model = machine;
        title.text(model->get_name());
      });
    }

  private:
    std::shared_ptr<Puppeteer::Machine> model;
    Crails::Front::Element title;
  };
}

#endif
