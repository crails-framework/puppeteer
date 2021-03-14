#ifndef  FRONT_MODAL_HPP
# define FRONT_MODAL_HPP

# include "html/views/resources/modal.hpp"
# include <comet/promise.hpp>
# include <comet/globals.hpp>
# include <iostream>

template<typename ELEMENT>
class Modal : public HtmlTemplate::Modal
{
  std::function<void()> on_resolved;
  bool accepted;
public:
  ELEMENT el;

  Modal()
  {
    std::cout << "Modal constructor" << std::endl;
    container > el;
  }

  virtual ~Modal()
  {
    std::cout << "Modal destroyed" << std::endl;
  }

  void bind_attributes()
  {
    HtmlTemplate::Modal::bind_attributes();
    el.bind_attributes();
  }

  void trigger_binding_updates()
  {
    HtmlTemplate::Modal::trigger_binding_updates();
    el.trigger_binding_updates();
  }

  static std::shared_ptr<Modal> make(const std::string& title)
  {
    auto self = std::make_shared<Modal>();

    self->set_title(title);
    self->bind_attributes();
    return self;
  }

  bool ok() const
  {
    return accepted;
  }

  Comet::Promise open()
  {
    return Comet::Promise([this](std::function<void()> resolve, std::function<void()> reject)
    {
      on_resolved = resolve;
      //Comet::body > *this;
      Comet::window.set("_comet_", (Comet::Object)(*this));
      client::window.eval("document.body.appendChild(window._comet_);");
    }).then([this]()
    {
      destroy();
    });
  }

  virtual void on_accepted()
  {
    accepted = true;
    on_resolved();
  }

  virtual void on_rejected()
  {
    accepted = false;
    on_resolved();
  }
};

#endif
