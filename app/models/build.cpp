#include "build.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/getenv.hpp>
#include <crails/password.hpp>
#include "variable_list.hpp"

using namespace std;

odb_instantiable_impl(Build)

const std::string Build::builds_path = Crails::getenv("PUPPETEER_BUILDS_PATH", "/opt/puppeteer/builds");

void Build::collect_variables(map<string,string>& variables) const
{
  const VariableList local_variables = get_variables();

  variables["BUILD_NAME"]    = get_name();
  variables["BUILD_PATH"]    = get_build_path();
  variables["BUILD_OPTIONS"] = get_options();
  local_variables.to_map(variables);
}

std::string Build::get_build_path() const
{
  std::stringstream stream;
  std::stringstream options_stream;
  Crails::Password  options_hash;

  options_stream
    << get_git()
    << get_branch()
    << get_options();
  options_hash = options_stream.str();
  stream << builds_path << '/' << get_name() << '_' << options_hash.c_str();
  return stream.str();
}
