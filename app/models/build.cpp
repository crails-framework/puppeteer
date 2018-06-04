#include "build.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/getenv.hpp>
#include <crails/password.hpp>
#include "variable_list.hpp"
#include "app/jenkins/jenkins.hpp"

#include <crails/read_file.hpp>

using namespace std;

odb_instantiable_impl(Build)

const std::string Build::builds_path = Crails::getenv("PUPPETEER_BUILDS_PATH", "/opt/puppeteer/builds");

void Build::collect_variables(map<string,string>& variables) const
{
  const VariableList local_variables = get_variables();

  variables["BUILD_NAME"]       = get_name();
  variables["BUILD_PATH"]       = get_build_path();
  variables["BUILD_OPTIONS"]    = get_options();
  variables["BUILD_GIT_URL"]    = get_git();
  variables["BUILD_GIT_BRANCH"] = get_branch();
  local_variables.to_map(variables);
}

std::string Build::get_build_path() const
{
  std::stringstream stream;

  stream << builds_path << '/' << get_name() << '_' << get_id();
  return stream.str();
}

std::string generate_file_from_template(const std::string& template_path, std::map<std::string,std::string> variables)
{
  std::string content;

  if (Crails::read_file(template_path, content))
  {
    for (auto variable : variables)
    {
      const string pattern = "#{" + variable.first + '}';
      size_t pos;

      while ((pos = content.find(pattern)) != string::npos)
        content.replace(pos, pattern.length(), variable.second);
    }
  }
  return content;
}

std::string Build::get_build_config()
{
  map<string,string> variables;
  auto recipe = get_recipe();
  auto source_path = recipe->get_path() + "/jenkins.xml";

  collect_variables(variables);
  return generate_file_from_template(source_path, variables);
}

void Build::on_change()
{
  Jenkins jenkins;
  int status = jenkins.push_config(get_name(), get_build_config());

  if (status != 200)
  {
    std::cout << "responded with status " << status << std::endl;
    throw std::runtime_error("could not create/update jenkins job");
  }
}
