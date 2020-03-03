#ifndef  VIEW_NOT_FOUND_HPP
# define VIEW_NOT_FOUND_HPP

# include <crails/front/custom_element.hpp>
# include "front/resources/modal.hpp"

class NotFoundElement : public Crails::Front::CustomElement
{
public:
  NotFoundElement()
  {
    text("The specified route does not match any views.");
  }
};

class NotFoundModal : public Modal<NotFoundElement>
{
public:
  NotFoundModal()
  {
    set_yes_label("Ok");
    set_no_label("");
  }

  static std::shared_ptr<NotFoundModal> make_and_open(const std::string&)
  {
    auto modal = std::make_shared<NotFoundModal>();

    modal->set_title("Not found");
    modal->bind_attributes();
    modal->open().then([modal]() {});
    return modal;
  }
};

#endif
