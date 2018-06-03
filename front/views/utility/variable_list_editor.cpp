#include "variable_list_editor.hpp"

using namespace Views;

VariableListEditor::VariableListEditor()
{
  button_add.text("Add variable");
  button_add.add_class("btn-primary");
  listen_to(button_add.clicked, std::bind(&VariableListEditor::on_add_clicked, this, std::placeholders::_1));
}

void VariableListEditor::render()
{
  auto variable_map = variables.to_map();
  std::vector<Crails::Front::Element> fields;

  inputs.clear();
  View::render();
  el.html("");

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
      bind_event_listener(label, "change", std::bind(&VariableListEditor::on_change, this, std::placeholders::_1));
      bind_event_listener(value, "change", std::bind(&VariableListEditor::on_change, this, std::placeholders::_1));
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
  }
  el.inner(fields);
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
