#include "_log_widget.hpp"
#include <comet/http.hpp>
#include <comet/globals.hpp>

using namespace Comet;
using namespace std;
  
LogWidget::LogWidget() : last_line_count(0)
{
  attr("style", "background-color:#263238;color:white;padding:5px;");
}

void LogWidget::set_model(ModelPtr value)
{
  if (model != value)
  {
    model = value;
    if (model)
    {
      signaler.trigger("model-changed");
      fetch_logs();
    }
  }
}

void LogWidget::fetch_logs()
{
  auto request = Http::Request::get(model->get_url() + "/logs");

  request->set_headers({{"Accept", Archive::mimetype}});
  request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      IArchive     archive;
      unsigned int new_line_count;
      std::string  text;

      archive.set_data(response->get_response_text());
      archive & new_line_count & text;
      append_new_lines(text, new_line_count);
    }
    if (is_attached())
      Comet::window.set_timeout([this]() { fetch_logs(); }, 5000);
  });
}

void LogWidget::append_new_lines(const std::string& text, unsigned int new_line_count)
{
  auto lines = Crails::split(text, '\n');
  auto it = lines.begin();

  if (last_line_count > new_line_count)
    last_line_count = 0;
  if (last_line_count != 0)
  {
    unsigned int skip_count = lines.size() - (new_line_count - last_line_count);

    std::advance(it, skip_count);
  }
  last_line_count = new_line_count;
  for (; it != lines.end() ; ++it)
    *this > Comet::Element("div", {{"class","console-output-line console-wide-line"}}).text(*it);
}

void LogWidget::scroll_to_bottom()
{
  if (has_parent())
  {
    auto parent = get_parent();

    parent->set_scrollTop(parent->get_scrollHeight());
  }
}
