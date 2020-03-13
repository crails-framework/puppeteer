#include "new.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"

using namespace std;

Views::VariableSetForm::VariableSetForm()
{
}

Views::VariableSetForm::~VariableSetForm()
{
  std::cout << "Destroying variable set form" << std::endl;
}

void Views::VariableSetForm::initialize_breadcrumbs()
{
  Breadcrumbs::reset();
  Breadcrumbs::add_crumb("Variables", model->get_path());
  Breadcrumbs::done();
}

void Views::VariableSetForm::activate()
{
  variables_input.render();
  ModelForm::activate(0);
}

void Views::VariableSetForm::set_model(std::shared_ptr<Front::VariableSet> _model)
{
  ModelForm::set_model(_model);
  variables_input.set_value(model->get_value());
  variables_input.render();
  if (model)
    initialize_breadcrumbs();
}

void Views::VariableSetForm::update_model_attributes()
{
  model->set_value(variables_input.get_value());
}
