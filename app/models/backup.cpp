#include "backup.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "app/jenkins/jenkins.hpp"
# include "app/recipe_runners/backup_runner.hpp"
# include <crails/server.hpp>
# include <crails/getenv.hpp>
# include <boost/filesystem.hpp>
#endif

using namespace std;
using namespace Crails;

odb_instantiable_impl(Backup)

std::string Backup::get_name()
{
  stringstream stream;

  stream << get_instance()->get_name();
  stream << "-backup-" << get_id();
  return stream.str();
}

#ifndef  __CHEERP_CLIENT__
const string Backup::base_path = Crails::getenv("PUPPETEER_BACKUP_PATH", "/opt/puppeteer/backups");

std::string Backup::get_build_config()
{
  return std::string() + "<project>" +
  "<description/>" +
  "<keepDependencies>false</keepDependencies>" +
  "<properties/>" +
  "<scm class=\"hudson.scm.NullSCM\"/>" +
  "<canRoam>true</canRoam>" +
  "<disabled>false</disabled>" +
  "<blockBuildWhenDownstreamBuilding>false</blockBuildWhenDownstreamBuilding>" +
  "<blockBuildWhenUpstreamBuilding>false</blockBuildWhenUpstreamBuilding>" +
  "<triggers>" +
  "<hudson.triggers.TimerTrigger>" +
  "<spec>" + get_schedule() + "</spec>" +
  "</hudson.triggers.TimerTrigger>" +
  "</triggers>" +
  "<concurrentBuild>false</concurrentBuild>" +
  "<builders>" +
  "<hudson.tasks.Shell>" +
  "<command>" + get_command() + " </command>" +
  "</hudson.tasks.Shell>" +
  "</builders>" +
  "<publishers/>" +
  "<buildWrappers/>" +
  "</project>";
}

std::string Backup::get_command() const
{
  stringstream stream;

  stream << "cd '" << boost::filesystem::current_path().string() << "'\n";
  stream << ". ../env\n";
  stream << "bin/tasks/backup/task " << get_id() << ' ' << "$BUILD_NUMBER";
  return stream.str();
}

std::string Backup::get_backup_folder()
{
  return base_path + '/' + get_name();
}

void Backup::on_change()
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

void Backup::perform(Action action, const std::string& filename, Sync::Task& task)
{
  auto instance = get_instance();

  instance->open_ssh([this, instance, action, filename, &task](Ssh::Session& ssh)
  {
    auto         recipe = instance->get_build()->get_recipe();
    BackupRunner runner(ssh, *this, *instance, *recipe, task);
    string       tarball_path = get_backup_folder() + '/' + filename + ".tar.gz";

    runner.upload_variables();
    runner.upload_backup_script(action);
    if (action == RestoreAction)
      runner.push_tarball(tarball_path);
    runner.run_backup_script();
    if (action == BackupAction)
    {
      if (boost::filesystem::exists(get_backup_folder())
       || boost::filesystem::create_directory(get_backup_folder()))
        runner.pull_tarball(tarball_path);
      else
        throw runtime_error("Cannot create directory " + get_backup_folder());
    }
    runner.cleanup();
  });
}

std::string Backup::get_tarball_path_for_backup(unsigned int number)
{
  return get_tarball_path_for_backup(boost::lexical_cast<std::string>(number));
}

std::string Backup::get_tarball_path_for_backup(const std::string& number)
{
  return get_backup_folder() + '/' + number + ".tar.gz";
}

void Backup::clean_up_backup_folder()
{
  if (max_history != 0)
  {
    Jenkins jenkins;
    DataTree data = jenkins.get_project_data(get_name());
    unsigned short count = 0;

    data["builds"].each([this, &count](Data build) -> bool
    {
      if (count > max_history)
        remove_build(build["number"].as<unsigned int>());
      count += 1;
      return true;
    });
  }
}

void Backup::remove_build(unsigned int build_id)
{
  Jenkins jenkins;
  int     status       = jenkins.delete_build(get_name(), build_id);
  auto    tarball_path = get_tarball_path_for_backup(build_id);

  if (status >= 400)
    logger << Logger::Error << "Failed to remove jenkins log for backup " << get_id() << " build's " << build_id << Logger::endl;
  if (boost::filesystem::exists(tarball_path))
    boost::filesystem::remove(tarball_path);
}

string Backup::get_backup_script() const
{
  stringstream stream;

  stream << "$APP_PATH/backup.sh" << std::endl;
  return stream.str();
}

string Backup::get_restore_script() const
{
  stringstream stream;

  stream << "$APP_PATH/restore.sh" << std::endl;
  return stream.str();
}

bool Backup::can_destroy() const
{
  return true;
}

void Backup::before_destroy()
{
}
#endif
