#include "application.hpp"
#include <comet/globals.hpp>
#include <iostream>

using namespace Comet;

void webMain()
{
  std::cout << "(!) Starting Comet application..." << std::endl;
  Application::start();
}
