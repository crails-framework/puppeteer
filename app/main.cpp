#include <crails/server.hpp>
#include <crails/renderer.hpp>
#include <crails/cipher.h>
#include <crails/cookie_data.hpp>
#include "app/git/git.hpp"

using namespace std;
using namespace Crails;

int main(int argc, char **argv)
{
  Git::GlobalState git_state;

  // Initializers
  Renderer::initialize();

  // Application loop
  Server::Launch(argc, argv);

  // Finalizers
  Renderer::finalize();
  return (0);
}
