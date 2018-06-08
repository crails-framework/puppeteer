#ifndef  CONSOLE_OUTPUT_WIDGET_HPP
# define CONSOLE_OUTPUT_WIDGET_HPP

# include <crails/utils/string.hpp>
# include <crails/front/element.hpp>

namespace Views
{
  class ConsoleOutput : public Crails::Front::Element
  {
    std::stringstream buffer;
  public:
    ConsoleOutput()
    {
      attr("style", "background-color:#263238;color:white;padding:5px;");
    }

    void flush()
    {
      buffer.str(std::string());
      html("");
      //visible(false);
    }

    template<typename T>
    ConsoleOutput& operator<<(const T& value)
    {
      buffer << value;
      if (buffer.str().find('\n') != std::string::npos)
        append_new_lines();
      return *this;
    }

  private:
    void append_new_lines()
    {
      auto lines = Crails::split(buffer.str(), '\n');

      for (auto line : lines)
        *this > Crails::Front::Element("div", {{"class","console-output-line"}}).text(line);
      //visible(true);
      buffer.str(std::string());
    }
  };
}

#endif
