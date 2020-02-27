#include <crails/renderers/archive_renderer.hpp>

#include <crails/front/archive.hpp>
#include "app/models/machine.hpp"

using namespace Crails;
using namespace std;

std::string render_machine_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_machine_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_build_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_build_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_instance_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_instance_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_recipe_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_recipe_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_credential_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_credential_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_backup_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_backup_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_variable_set_show_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);
std::string render_variable_set_index_archive(const Crails::Renderer* renderer, Crails::SharedVars& vars);

ArchiveRenderer::ArchiveRenderer()
{
  templates.insert(
    pair<string, Generator>("machine_show", render_machine_show_archive)
  );
  templates.insert(
    pair<string, Generator>("machine_index", render_machine_index_archive)
  );
  templates.insert(
    pair<string, Generator>("build_show", render_build_show_archive)
  );
  templates.insert(
    pair<string, Generator>("build_index", render_build_index_archive)
  );
  templates.insert(
    pair<string, Generator>("instance_show", render_instance_show_archive)
  );
  templates.insert(
    pair<string, Generator>("instance_index", render_instance_index_archive)
  );
  templates.insert(
    pair<string, Generator>("recipe_show", render_recipe_show_archive)
  );
  templates.insert(
    pair<string, Generator>("recipe_index", render_recipe_index_archive)
  );
  templates.insert(
    pair<string, Generator>("credential_show", render_credential_show_archive)
  );
  templates.insert(
    pair<string, Generator>("credential_index", render_credential_index_archive)
  );
  templates.insert(
    pair<string, Generator>("backup_show", render_backup_show_archive)
  );
  templates.insert(
    pair<string, Generator>("backup_index", render_backup_index_archive)
  );
  templates.insert(
    pair<string, Generator>("variable_set_show", render_variable_set_show_archive)
  );
  templates.insert(
    pair<string, Generator>("variable_set_index", render_variable_set_index_archive)
  );
}
