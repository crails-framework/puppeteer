#ifndef  CONSOLE_OUTPUT_WIDGET_HPP
# define CONSOLE_OUTPUT_WIDGET_HPP

# include <crails/utils/string.hpp>
# include <comet/custom_element.hpp>

class ConsoleOutput : public Comet::CustomElement
{
  std::stringstream buffer;
public:
  ConsoleOutput()
  {
  }

  virtual ~ConsoleOutput() {}

  void flush()
  {
    buffer.str(std::string());
    html("");
  }

  template<typename T>
  ConsoleOutput& operator<<(const T& value)
  {
    buffer << value;
    if (buffer.str().find('\n') != std::string::npos)
      append_new_lines();
    return *this;
  }

  void append_final_line(const std::string& message)
  {
    append_new_lines();
    *this > Comet::Element("div", {{"class","console-output-line console-result-line"}}).text(message);
    scroll_to_bottom();
  }

private:
  void append_new_lines()
  {
    auto lines = Crails::split(buffer.str(), '\n');

    for (auto line : lines)
      *this > Comet::Element("div", {{"class","console-output-line"}}).text(line);
    buffer.str(std::string());
    scroll_to_bottom();
  }

  void scroll_to_bottom()
  {
    if (has_parent())
    {
      auto parent = get_parent();

      parent->set_scrollTop(parent->get_scrollHeight());
    }
  }
};

#endif
