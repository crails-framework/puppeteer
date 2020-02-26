#include "build.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "instance.hpp"
# include <crails/password.hpp>
# include <crails/getenv.hpp>
# include <crails/read_file.hpp>
# include "app/jenkins/jenkins.hpp"
# include <boost/filesystem.hpp>
# include <crails/utils/string.hpp>
#endif
#include "variable_list.hpp"

using namespace std;

odb_instantiable_impl(Build)

#ifndef __CHEERP_CLIENT__
std::vector<std::string> list_directory(const std::string& directory_path);

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

  stream << "chmod g+w \"$BUILD_PATH/$BUILD_NUMBER.tar.gz\"\n";
  stream << "cd '" << boost::filesystem::current_path().string() << "'\n";
  stream << ". ../env\n";
  stream << "bin/tasks/auto-deploy-for/task " << build_id;
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

void Build::remove_build(unsigned int build_id)
{
  Jenkins jenkins;
  int status = jenkins.delete_build(get_name(), build_id);

  if (status >= 400)
  {
    std::cout << "responded with status " << status << std::endl;
    throw std::runtime_error("could not remove build from jenkins");
  }

  stringstream tarball_path_stream;
  string tarball_path;

  tarball_path_stream << get_build_path() << '/' << build_id << ".tar.gz";
  tarball_path = tarball_path_stream.str();
  if (boost::filesystem::exists(tarball_path))
    boost::filesystem::remove(tarball_path);
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

static bool string_ends_with(std::string const &fullString, std::string const &ending)
{
  if (fullString.length() >= ending.length())
  {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  return false;
}

vector<string> Build::get_available_builds() const
{
  auto files = list_directory(get_build_path());
  vector<string> filenames;

  for (const auto& filepath : files)
  {
    if (string_ends_with(filepath, ".tar.gz"))
    {
      string filename = *(Crails::split(filepath, '/').rbegin());

      filenames.push_back(filename.substr(0, filename.length() - 7));
    }
  }
  std::sort(filenames.begin(), filenames.end(), [](const std::string& a, const std::string& b) -> bool
  {
    stringstream str_a; stringstream str_b;
    unsigned int int_a, int_b;

    str_a << a;     str_b << b;
    str_a >> int_a; str_b >> int_b;
    return int_a > int_b;
  });
  return filenames;
}

void log_destroy_failure_on_bounded_resources(const std::string& resource, unsigned long count, const std::string& bound_resources);

bool Build::can_destroy() const
{
  auto& database = *(ODB::Connection::instance);
  unsigned long bound_instance_count;

  bound_instance_count = database.count<Instance>(odb::query<Instance>::build_id == get_id());
  if (bound_instance_count)
    log_destroy_failure_on_bounded_resources("build `" + get_name() + '`', bound_instance_count, "instances");
  return bound_instance_count == 0;
}

void Build::before_destroy()
{
}
#endif
