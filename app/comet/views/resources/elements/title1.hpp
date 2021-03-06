#ifndef  CUSTOM_ELEMENT_TITLE1_HPP
# define CUSTOM_ELEMENT_TITLE1_HPP

# include "html/views/resources/elements/title1.hpp"
# include <iostream>

class Title1 : public HtmlTemplate::Title1
{
public:
  virtual ~Title1() {}

  void set_title(const std::string& value)
  {
    std::cout << "<title-1> Set title " << value << std::endl;
    title = value;
    trigger_binding_updates();
  }
};

#endif
