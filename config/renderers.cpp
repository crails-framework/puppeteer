#include <crails/renderer.hpp>
#include <crails/renderers/archive_renderer.hpp>

using namespace Crails;

const std::string Renderer::default_format = "text/html";

void Renderer::initialize()
{
  // Append renderers
  renderers.push_back(new ArchiveRenderer);
}
