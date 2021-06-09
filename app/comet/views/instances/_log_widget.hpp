#ifndef  LOG_WIDGET_HPP
# define LOG_WIDGET_HPP

# include <comet/custom_element.hpp>
# include "app/comet/models/instance.hpp"

class LogWidget : public Comet::CustomElement
{
  typedef std::shared_ptr<Front::Instance> ModelPtr;
  ModelPtr model;
  unsigned int last_line_count;
public:
  LogWidget();
  virtual ~LogWidget() {}

  ModelPtr get_model() const { return model; }

  void set_model(ModelPtr value);
  void fetch_logs();

private:
  void append_new_lines(const std::string& text, unsigned int new_line_count);
  void scroll_to_bottom();
};

#endif
