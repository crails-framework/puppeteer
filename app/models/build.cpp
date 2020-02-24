#include "build.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "instance.hpp"
# include <crails/password.hpp>
# include <crails/getenv.hpp>
# include <crails/read_file.hpp>
# include "app/jenkins/jenkins.hpp"
# include <boost/filesystem.hpp>
#endif
#include "variable_list.hpp"

using namespace std;

odb_instantiable_impl(Build)

#ifndef __CHEERP_CLIENT__
const std::string Build::builds_path = Crails::getenv("PUPPETEER_BUILDS_PATH", "/opt/puppeteer/builds");

void Build::collect_variables(map<string,string>& variables)
{
  const VariableList local_variables = get_variables();
  auto credentials = get_credential();

  variables["BUILD_NAME"]       = get_name();
  variables["BUILD_PATH"]       = get_build_path();
  variables["BUILD_OPTIONS"]    = get_options();
  variables["BUILD_GIT_URL"]    = get_git();
  variables["BUILD_GIT_BRANCH"] = get_branch();
  if (credentials)
    variables["JENKINS_CREDENTIAL_ID"] = credentials->get_jenkins_id();
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

static std::string get_build_auto_deploy_command(ODB::id_type build_id)
{
  stringstream stream;

  stream << "cd '" << boost::filesystem::current_path().string() << "' &&";
  stream << ". ../env && bin/tasks/auto-deploy-for/task " << build_id;
  return stream.str();
}

std::string Build::get_build_config()
{
  map<string,string> variables;
  auto recipe = get_recipe();
  auto source_path = recipe->get_repository_path() + "/jenkins.xml";

  collect_variables(variables);
  variables["BUILD_AUTO_DEPLOY_COMMAND"] = get_build_auto_deploy_command(get_id());
  return generate_file_from_template(source_path, variables);
}

void Build::on_change()
{
  if (get_id() != ODB_NULL_ID)
  {
    Jenkins jenkins;
    int status = jenkins.push_config(get_name(), get_build_config());

    if (status != 200)
    {
      std::cout << "responded with status " << status << std::endl;
      throw std::runtime_error("could not create/update jenkins job");
    }
  }
}

bool Build::update_last_build()
{
  Jenkins jenkins;
  const DataTree data = jenkins.get_project_data(get_name());

  return update_last_build(data);
}

bool Build::update_last_build(const DataTree& data)
{
  const Data last_successful_build = data["lastSuccessfulBuild"]["number"];
  auto previous_last_build = get_last_build();

  if (last_successful_build.exists())
    set_last_build(last_successful_build);
  else
    set_last_build(0);
  return previous_last_build != get_last_build();
}
#endif
