#ifndef  FRONT_MODAL_HPP
# define FRONT_MODAL_HPP

# include "lib/cheerp-html/resources/modal.hpp"

template<typename ELEMENT>
class Modal : public HtmlTemplate::Modal
{
  std::function<void()> on_resolved;
  bool accepted;
public:
  ELEMENT el;

  Modal()
  {
    container > el;
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
    //self->signaler.trigger("modal-changed");
    return self;
  }

  bool ok() const
  {
    return accepted;
  }

  Crails::Front::Promise open()
  {
    return Crails::Front::Promise([this](std::function<void()> resolve, std::function<void()> reject)
    {
      on_resolved = resolve;
      Crails::Front::body > *this;
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
