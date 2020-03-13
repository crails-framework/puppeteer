#include "variables.hpp"
#include "app/comet/views/variable_sets/new.hpp"
#include <iostream>

using namespace std;
using namespace Comet;

VariablesController::VariablesController(const Params& p) : ApplicationController(p)
{
}

void VariablesController::show()
{
  auto view = std::make_shared<Views::VariableSetForm>();

  use_application_layout().render(view);
  view->activate();
  on_done();
}
