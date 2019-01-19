#include "recipe.hpp"
#include "app/models/variable_set.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include <crails/getenv.hpp>
# include <crails/utils/string.hpp>
# include <crails/sync/task.hpp>
# include "app/ssh/session.hpp"
# include "app/ssh/scp.hpp"
# include "app/git/git.hpp"
# include <boost/filesystem.hpp>
#endif

using namespace std;

odb_instantiable_impl(Recipe)

#ifndef __CHEERP_CLIENT__
const string Recipe::base_path   = Crails::getenv("PUPPETEER_RECIPE_PATH", "/opt/puppeteer/recipes");
const string Recipe::remote_path = Crails::getenv("PUPPETEER_REMOTE_PATH", "/opt/puppeteer/client");
const string Recipe::remote_user = Crails::getenv("PUPPETEER_REMOTE_USER", "root");

void Recipe::on_change()
{
  fetch_recipe();
}

std::vector<std::string> list_directory(const std::string& directory_path)
{
  std::vector<std::string> list;
  boost::filesystem::path p(directory_path.c_str());
  boost::filesystem::directory_iterator it(p), end_it;

  for (; it != end_it ; ++it)
  {
    if (boost::filesystem::is_regular_file(it->path()))
      list.push_back(it->path().string());
  }
  return list;
}

static bool is_same_repository(const std::string& path, const std::string& url)
{
  boost::filesystem::path p(path);

  if (boost::filesystem::is_directory(p))
  {
    Git::Repository repository;

    repository.open(path);
    return repository.find_remote("origin")->get_url() == url;
  }
  return false;
}

static std::string initialize_git_repository(const std::string& path, const std::string& url, const std::string& branch = "master")
{
  boost::filesystem::path p(path);
  Git::Repository repository;

  if (boost::filesystem::is_directory(p))
    repository.open(path);
  else
    repository.clone(url, path);
  repository.checkout(branch, GIT_CHECKOUT_FORCE);
  repository.find_remote("origin")->pull();
  return repository.get_tip_oid();
}

string Recipe::get_repository_path() const
{
  return base_path + '/' + get_name();
}

void Recipe::fetch_recipe()
{
  string repository_path = get_repository_path();
  string url = get_git_url();
  string oid;
  auto   credential = get_credential();

  if (credential)
  {
    size_t pos = url.find("://");

    url.replace(pos + 3, 0, credential->get_username() + ':' + credential->get_password() + '@');
  }
  if (!is_same_repository(repository_path, url))
    boost::filesystem::remove_all(repository_path);
  oid = initialize_git_repository(repository_path, url, get_git_branch());
  set_last_tip(oid);
}

static std::string generate_variable_file(const std::map<std::string, std::string>& variables)
{
  std::stringstream stream;

  for (const auto& variable : variables)
    stream << "export " << variable.first << '=' << '"' << variable.second << '"' << '\n';
  return stream.str();
}

void Recipe::exec_package(const std::string& package, Instance& instance, Sync::Task& task)
{
  Ssh::Session ssh;
  Sync::Stream stream(task);
  auto machine = instance.get_machine();
  auto build   = instance.get_build();

  ssh.should_accept_unknown_hosts(true);
  ssh.connect(remote_user, machine->get_ip());
  ssh.authentify_with_pubkey();
  task.increment();
  {
    const std::string remote_folder = remote_path + '/' + instance.get_name();
    const std::string recipe_folder = get_repository_path();
    const std::string recipe_package_folder = recipe_folder + '/' + package;
    const std::string remote_package_folder = remote_folder + '/' + package;
    auto package_files = list_directory(recipe_package_folder);
    std::map<std::string, std::string> variables;

    std::cout << "SET TASK COUNT: " << (package_files.size() + 4) << std::endl;
    task.set_task_count(package_files.size() + 4);
    ssh.exec("mkdir -p " + remote_package_folder, stream);

    // scp recipe
    {
      auto scp = ssh.make_scp_session(remote_package_folder, SSH_SCP_WRITE);

      for (auto file : package_files)
      {
        auto filename = (*Crails::split(file, '/').rbegin());

        scp->push_file(file, filename);
      }
      task.increment();
    }

    // scp ingredients
    {
      auto scp = ssh.make_scp_session(remote_package_folder, SSH_SCP_WRITE);

      VariableSet::collect_global_variables(variables);
      instance.collect_variables(variables);
      build->collect_variables(variables);
      variables["MACHINE_IP"] = machine->get_ip();
      scp->push_text(generate_variable_file(variables), "variables");
      task.increment();
    }

    // run recipe
    std::sort(package_files.begin(), package_files.end()); // must run files in an orderly fashion
    for (auto file : package_files)
    {
      auto filename  = (*Crails::split(file, '/').rbegin());
      auto extension = (*Crails::split(filename, '.').rbegin());

      if (extension == "sh")
      {
        int status;

        status = ssh.exec("chmod +x '" + remote_package_folder + '/' + filename + '\'', stream);
        if (status)
          throw std::runtime_error("Recipe(" + get_name() + "): could not chmod script: " + filename);
        stream << " \n" << "## " << filename << ":\n";
        status = ssh.exec("cd '" + remote_package_folder + "/' && ./" + filename + " 2>&1", stream);
        if (status)
        {
          std::stringstream stream;
          stream << "Recipe(" << get_name() << "): remote script `" << filename << "` returned with error status " << status;
          throw std::runtime_error(stream.str());
        }
      }
      task.increment();
    }

    // run plugins
    for (const auto plugin : plugins)
    {
      if (plugin->recipe_uses_plugin(recipe_folder))
        plugin->apply(package, recipe_folder, instance, variables, stream);
    }

    task.increment();
  }
}

void Recipe::deploy_for(Instance& instance, Sync::Task& task)
{
  exec_package("setup", instance, task);
}

void Recipe::uninstall_from(Instance& instance, Sync::Task& task)
{
  exec_package("uninstall", instance, task);
}
#endif
