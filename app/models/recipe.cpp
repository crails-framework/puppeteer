#include "recipe.hpp"
#include "lib/odb/application-odb.hxx"
#include <crails/getenv.hpp>
#include <crails/utils/string.hpp>
#include "app/ssh/session.hpp"
#include "app/ssh/scp.hpp"

odb_instantiable_impl(Recipe)

const std::string Recipe::base_path   = Crails::getenv("PUPPETEER_RECIPE_PATH", "/opt/puppeteer/recipes");
const std::string Recipe::remote_path = Crails::getenv("PUPPETEER_REMOTE_PATH", "/opt/puppeteer/client");

#include <boost/filesystem.hpp>

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

std::list<std::string> read_lines_from_file(const std::string& filename)
{
  std::ifstream stream(filename.c_str());
  std::list<std::string> lines;

  if (stream.is_open())
  {
    std::string line;

    while (getline(stream, line))
      lines.push_back(line);
    stream.close();
  }
  else
    throw std::runtime_error("file `" + filename + "` not found");
  return lines;
}

static std::string generate_variable_file(const std::map<std::string, std::string>& variables)
{
  std::stringstream stream;

  for (const auto& variable : variables)
    stream << "export " << variable.first << '=' << variable.second << '\n';
  return stream.str();
}

void Recipe::deploy_for(Instance& instance)
{
  Ssh::Session ssh;
  auto machine = instance.get_machine();
  auto build   = instance.get_build();

  ssh.should_accept_unknown_hosts(true);
  ssh.connect(instance.get_user(), machine->get_ip());
  ssh.authentify_with_pubkey();
  {
    const std::string remote_folder = remote_path + '/' + instance.get_name();
    const std::string recipe_folder = base_path   + '/' + instance.get_name();
    const std::string recipe_setup_folder = recipe_folder + "/setup";
    const std::string remote_setup_folder = remote_folder + "/setup";
    auto setup_files = list_directory(recipe_setup_folder);

    ssh.exec("mkdir -p " + remote_setup_folder, std::cout);

    // scp recipe
    {
      auto scp = ssh.make_scp_session(remote_setup_folder, SSH_SCP_WRITE);

      for (auto setup_file : setup_files)
      {
        auto filename = (*Crails::split(setup_file, '/').rbegin());

        scp->push_file(setup_file, filename);
      }
    }

    // scp ingredients
    {
      auto scp = ssh.make_scp_session(remote_setup_folder, SSH_SCP_WRITE);
      std::map<std::string, std::string> variables;

      instance.collect_variables(variables);
      build->collect_variables(variables);
      scp->push_text(generate_variable_file(variables), "variables");
    }

    // run recipe
    std::sort(setup_files.begin(), setup_files.end()); // must run files in an orderly fashion
    for (auto setup_file : setup_files)
    {
      auto filename  = (*Crails::split(setup_file, '/').rbegin());
      auto extension = (*Crails::split(filename, '.').rbegin());

      if (extension == "sh")
      {
        int status;

        status = ssh.exec("chmod +x '" + remote_setup_folder + '/' + filename + '\'', std::cout);
        if (status)
          throw std::runtime_error("Recipe(" + get_name() + "): could not chmod script: " + filename);
        status = ssh.exec("cd '" + remote_setup_folder + "/' && ./" + filename, std::cout);
        if (status)
        {
          std::stringstream stream;
          stream << "Recipe(" << get_name() << "): remote script `" << filename << "` returned with error status " << status;
          throw std::runtime_error(stream.str());
        }
      }
    }
  }
}
