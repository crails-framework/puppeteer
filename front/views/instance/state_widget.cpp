#include "state_widget.hpp"
#include <iostream>

using namespace Views;
using namespace std;

typedef Crails::Front::Element El;

static std::string strip_instance_name(const std::string& name, const Puppeteer::Instance& instance)
{
  size_t length = instance.get_name().length();

  if (name.find(instance.get_name() + '_') != std::string::npos ||
      name.find(instance.get_name() + '-') != std::string::npos)
    return name.substr(length + 1);
  return name;
}

InstanceStateWidget::InstanceStateWidget()
{
  text("Loading...");
}

void InstanceStateWidget::activate(std::shared_ptr<Puppeteer::Instance> instance)
{
  model = instance;
  if (model->get_state() > 0)
    fetch_state();
  else
    render_uninstalled_state();
}

void InstanceStateWidget::fetch_state()
{
  Crails::Front::Ajax::query("GET", model->get_url() + "/state").callbacks({
    std::bind(&InstanceStateWidget::on_state_fetched,      this, std::placeholders::_1),
    std::bind(&InstanceStateWidget::on_state_fetch_failed, this, std::placeholders::_1)
  })();
}

void InstanceStateWidget::render_uninstalled_state()
{
  html("").inner({
    make_configuration_state_label(),
    El("div").text("This instance isn't configured on the remote server.")
  });
}

El InstanceStateWidget::make_configuration_state_label()
{
  El configuration_state("span", {{"class","role"}});

  switch (model->get_state())
  {
  case 0:
    configuration_state.text("Uninstalled").add_class("user");
    break ;
  case 1:
    configuration_state.text("Ready").add_class("member");
    break ;
  case 2:
    configuration_state.text("Dirty").add_class("admin");
    break ;
  default:
    configuration_state.text("Unknown");
    break ;
  }
  return El("div").inner({
    El("label").attr("style","margin-right:10px").text("Configuration :"),
    configuration_state
  });
}

void InstanceStateWidget::render_state()
{
  // Process state
  auto states = state.get_states();
  vector<El> elements;

  for (auto process : states)
  {
    elements.push_back(El("tr").inner({
      El("td").text(strip_instance_name(process.first, *model)),
      El("td").text(process.second)
    }));
  }

  // Render
  html("");
  inner({
    make_configuration_state_label(),
    El("table", {{"class","table table-borderless"}}).inner({
      El("thead").inner({ El("tr").inner({ El("td").text("Process"), El("td").text("State")  }) }),
      El("tbody").inner(elements)
    })
  });
}

void InstanceStateWidget::on_state_fetched(const Crails::Front::Ajax& ajax)
{
  std::cout << "successfully fetched state" << std::endl;
  if (ajax->get_responseText())
  {
    Crails::Front::Object responseText(ajax->get_responseText());
    IArchive              archive;

    archive.set_data(responseText);
    state.serialize(archive);
    render_state();
  }
}

void InstanceStateWidget::on_state_fetch_failed(const Crails::Front::Ajax& ajax)
{
  std::cout << "failed to fetch state" << std::endl;
  html("").text("Failed to fetch instance state");
}
