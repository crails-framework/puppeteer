#ifndef  VIEW_INSTANCE_HPP
# define VIEW_INSTANCE_HPP

# include "utility/template_view.hpp"
# include "utility/theme.hpp"
# include "utility/button.hpp"
# include <iostream>
# include "../app/instances.hpp"

namespace Views
{
  class InstanceStateWidget : public Crails::Front::Element
  {
    std::shared_ptr<Puppeteer::Instance> model;
  public:
    void activate(std::shared_ptr<Puppeteer::Instance> instance)
    {
      model = instance;
    }
  };

  class InstanceActionWidget : public Crails::Front::Element, public Crails::Listener
  {
    std::shared_ptr<Puppeteer::Instance> model;
    Button button_deploy;
  public:
    InstanceActionWidget()
    {
      button_deploy.add_class("btn-warning");
      button_deploy.text("Deploy");
      listen_to(button_deploy.clicked, std::bind(&InstanceActionWidget::deploy, this, std::placeholders::_1));
    }

    void activate(std::shared_ptr<Puppeteer::Instance> instance)
    {
      model = instance;
      inner({ button_deploy });
    }

    void deploy(client::Event*)
    {
      std::cout << "make ajax query for deployment here" << std::endl;
    }
  };

  class Instance : public TemplateView
  {
    InstanceStateWidget  state_widget;
    InstanceActionWidget action_widget;
    Crails::Front::Element edit_link;
  public:
    Instance() : TemplateView("tpl1")
    {
      title = El("h3");
      edit_link = El("a", {{"class","btn btn-lg btn-info"}}).text("Edit");

      el.add_class("container-fluid");
      el.inner({
        Theme::title1(El("div").inner({
          El("h2", {{"class","title-1"}}).text("Instance"),
          title
        }), edit_link),
        El("div", {{"class","row"}}).inner({
          El("div", {{"class","col-lg-6"}}).inner({
            Theme::card("State", state_widget)
          }),
          El("div", {{"class","col-lg-6"}}).inner({
            Theme::card("Actions", action_widget)
          })
        })
      });
    }

    void activate(unsigned long instance_id)
    {
      edit_link.attr("href", "#/instances/" + boost::lexical_cast<std::string>(instance_id) + "/edit");
      fetch_one<Puppeteer::Instance>(instance_id, [this](std::shared_ptr<Puppeteer::Instance> instance)
      {
        std::cout << "Fetched instance, finish activation" << std::endl;
        model = instance;
        title.text(model->get_name());
        state_widget.activate(model);
        action_widget.activate(model);
        std::cout << "what the fucking shit" << std::endl;
      });
    }

  private:
    std::shared_ptr<Puppeteer::Instance> model;
    Crails::Front::Element title;
  };
}

#endif
