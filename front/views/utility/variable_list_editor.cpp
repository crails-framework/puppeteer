#include "variable_list_editor.hpp"
#include <iostream>

using namespace Views;

typedef Crails::Front::Element El;

VariableListEditor::VariableListEditor()
{
  button_add.text("Add variable");
  button_add.add_class("btn-primary");
  listen_to(button_add.clicked, std::bind(&VariableListEditor::on_add_clicked, this, std::placeholders::_1));
  std::cout << "VariableListEditor constructor" << std::endl;
}

void VariableListEditor::render()
{
  auto variable_map = variables.to_map();
  std::vector<Crails::Front::Element> fields;

  inputs.clear();
  html("");
  event_listeners.clear();

  std::cout << "VariableListEditor::render()" << std::endl;

  if (!read_only)
    variable_map[""] = "";
  for (auto entry : variable_map)
  {
    El label("input", {{"type","text"},{"class","form-control"},{"placeholder","key"}});
    El value("input", {{"type","text"},{"class","form-control"},{"placeholder","value"}});

    label.value(entry.first);
    value.value(entry.second).attr("style","display:inline-block;width:70%");
    inputs.push_back(std::pair<El,El>(label, value));
    fields.push_back(El("div", {{"class","form-group"}}).inner({
      El("label", {{"class","form-control-label"},{"style","width:30%"}}).inner({label}),
      value
    }));
    if (read_only)
    {
      label.attr("readonly","true");
      value.attr("readonly","true");
    }
    else
    {
      label.events->on("change", [this](client::Event* e) { on_change(e); });
      value.events->on("change", [this](client::Event* e) { on_change(e); });
      event_listeners.push_back(label.events);
      event_listeners.push_back(value.events);
    }
  }
  if (!read_only)
  {
    fields.push_back(
      El("div",{{"class","controls pull-right"}}).inner({
        button_add
      })
    );
    fields.push_back(El("div",{{"style","clear:both;"}}));
    std::cout << "VariableListEditor Adding button" << std::endl;
  }
  inner(fields);
  std::cout << "VariableListEditor render over" << std::endl;
}


void VariableListEditor::on_add_clicked(client::Event*)
{
  update_value();
  render();
}

void VariableListEditor::on_change(client::Event*)
{
  update_value();
}

void VariableListEditor::update_value()
{
  std::map<std::string, std::string> value;

  for (auto entry : inputs)
    value[entry.first.get_value()] = entry.second.get_value();
  value.erase("");
  variables.from_map(value);
}
