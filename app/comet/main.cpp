#include "application.hpp"
#include <comet/globals.hpp>
#include <iostream>

using namespace Comet;

void webMain()
{
  std::cout << "Coucou petite comete" << std::endl;
  Application::start();
  Application::get().router.start();
}
