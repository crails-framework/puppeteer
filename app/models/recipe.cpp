#include "recipe.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/getenv.hpp>
#include <crails/utils/string.hpp>
#include "app/ssh/session.hpp"
#include "app/ssh/scp.hpp"
#include "app/git/git.hpp"
#include <boost/filesystem.hpp>

using namespace std;

odb_instantiable_impl(Recipe)

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

static bool has_nginx_configuration(const std::string& recipe_path)
{
  std::string nginx_conf_path = recipe_path + "/nginx.conf";
  boost::filesystem::path p(nginx_conf_path.c_str());

  return boost::filesystem::is_regular_file(p);
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

static void initialize_git_repository(const std::string& path, const std::string& url, const std::string& branch = "master")
{
  boost::filesystem::path p(path);
  Git::Repository repository;

  std::cout << "GIT PATH:   " << path << std::endl;
  std::cout << "GIT URL IS: " << url << std::endl;
  if (boost::filesystem::is_directory(p))
  {
    std::cout << "Opening repository" << std::endl;
    repository.open(path);
  }
  else
  {
    std::cout << "Cloning repository" << std::endl;
    repository.clone(url, path);
  }
  std::cout << "Checking out repository" << std::endl;
  repository.checkout(branch, GIT_CHECKOUT_FORCE);
  std::cout << "Pulling repository" << std::endl;
  repository.find_remote("origin")->pull();
}

string Recipe::get_path() const
{
  return base_path   + '/' + get_name();
}

void Recipe::fetch_recipe()
{
  string repository_path = get_path();
  string url = get_git_url();
  auto   credential = get_credential();

  if (credential)
  {
    size_t pos = url.find("://");

    url.replace(pos + 3, 0, credential->get_username() + ':' + credential->get_password() + '@');
  }
  if (!is_same_repository(repository_path, url))
    boost::filesystem::remove_all(repository_path);
  initialize_git_repository(repository_path, url, get_git_branch());
}

static std::string generate_variable_file(const std::map<std::string, std::string>& variables)
{
  std::stringstream stream;

  for (const auto& variable : variables)
    stream << "export " << variable.first << '=' << '"' << variable.second << '"' << '\n';
  return stream.str();
}

void Recipe::exec_package(const std::string& package, Instance& instance)
{
  Ssh::Session ssh;
  auto machine = instance.get_machine();
  auto build   = instance.get_build();

  ssh.should_accept_unknown_hosts(true);
  ssh.connect(remote_user, machine->get_ip());
  ssh.authentify_with_pubkey();
  {
    const std::string remote_folder = remote_path + '/' + instance.get_name();
    const std::string recipe_folder = get_path();
    const std::string recipe_package_folder = recipe_folder + '/' + package;
    const std::string remote_package_folder = remote_folder + '/' + package;
    auto package_files = list_directory(recipe_package_folder);

    ssh.exec("mkdir -p " + remote_package_folder, std::cout);

    // scp recipe
    {
      auto scp = ssh.make_scp_session(remote_package_folder, SSH_SCP_WRITE);

      for (auto file : package_files)
      {
        auto filename = (*Crails::split(file, '/').rbegin());

        scp->push_file(file, filename);
      }
    }

    // scp ingredients
    {
      auto scp = ssh.make_scp_session(remote_package_folder, SSH_SCP_WRITE);
      std::map<std::string, std::string> variables;

      instance.collect_variables(variables);
      build->collect_variables(variables);
      variables["MACHINE_IP"] = machine->get_ip();
      scp->push_text(generate_variable_file(variables), "variables");
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

        status = ssh.exec("chmod +x '" + remote_package_folder + '/' + filename + '\'', std::cout);
        if (status)
          throw std::runtime_error("Recipe(" + get_name() + "): could not chmod script: " + filename);
        status = ssh.exec("cd '" + remote_package_folder + "/' && ./" + filename, std::cout);
        if (status)
        {
          std::stringstream stream;
          stream << "Recipe(" << get_name() << "): remote script `" << filename << "` returned with error status " << status;
          throw std::runtime_error(stream.str());
        }
      }
    }

    // deploy gate configuration
    if (has_nginx_configuration(recipe_folder))
    {
      std::string nginx_server_ip = Crails::getenv("NGINX_SERVER_IP");

      if (nginx_server_ip.length())
      {
        Ssh::Session ssh;

        ssh.should_accept_unknown_hosts(true);
        ssh.connect(remote_user, nginx_server_ip);
        ssh.authentify_with_pubkey();
        ssh.make_scp_session("/etc/nginx/sites-available", SSH_SCP_WRITE)->push_file(recipe_folder + "/nginx.conf", instance.get_name());
      }
    }
  }
}

void Recipe::deploy_for(Instance& instance)
{
  exec_package("setup", instance);
}

void Recipe::uninstall_from(Instance& instance)
{
  exec_package("uninstall", instance);
}
