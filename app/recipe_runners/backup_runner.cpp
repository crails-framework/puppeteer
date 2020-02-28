#include "backup_runner.hpp"
#include <crails/utils/string.hpp>
#include <sstream>

using namespace std;

const string BackupRunner::backup_script_name = "backup";

BackupRunner::BackupRunner(Ssh::Session& s, Backup& b, Instance& i, Recipe& r, Sync::Task& t)
  : ScriptRunner(s, r, i, t), backup(b)
{
}

void BackupRunner::upload_backup_script(Backup::Action action)
{
  require_remote_folder();
  auto scp = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);
  stringstream script;

  script << "#!/bin/bash" << std::endl << std::endl;
  script << "source " << get_variable_filepath() << std::endl;
  switch (action)
  {
  case Backup::BackupAction:
    generate_backup_path();
    script << "export BACKUP_DIR=\"" << get_remote_folder() << "/output\"" << std::endl;
    script << "mkdir -p \"$BACKUP_DIR\"" << std::endl;
    script << "chmod 777 \"$BACKUP_DIR\"" << std::endl;
    script << backup.get_backup_script() << std::endl;
    script << "cd \"$BACKUP_DIR\"" << std::endl;
    script << "tar -czvf " << remote_backup_path << " * > /dev/null" << std::endl;
    script << "rm -Rf \"" << get_remote_folder() << "\"" << std::endl;
    break ;
  case Backup::RestoreAction:
    script << "export RESTORE_TARBALL=\"";
    script << get_remote_folder() << '/' << "restore.tar.gz";
    script << '"' << std::endl;
    script << backup.get_restore_script();
    break ;
  }

  std::cout << "DEBUG DATA" << std::endl;
  std::cout << "Instance id: " << instance.get_id() << ", name: " << instance.get_name() << std::endl;
  std::cout << "Remote folder: " << get_remote_folder() << std::endl;
  std::cout << "Script:" << std::endl << script.str() << std::endl << std::endl;
  std::cout << "END DEBUG DATA" << std::endl;

  stream << "Uploading " << backup_script_name << ".sh script...\n";
  scp->push_text(script.str(), backup_script_name + ".sh");
  stream << "Done.\n";
  task.increment();
}

void BackupRunner::run_backup_script()
{
  run_script(backup_script_name);
}

void BackupRunner::push_tarball(const std::string& source)
{
  auto scp = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);

  stream << "Uploading " << source << "...\n";
  scp->push_file(source, "restore.tar.gz");
  stream << "Done.\n";
  task.increment();
}

void BackupRunner::pull_tarball(const std::string& target)
{
  auto scp = ssh.make_scp_session(remote_backup_path, SSH_SCP_READ);

  stream << "Downloading " << remote_backup_path << "...\n";
  scp->pull_file(target);
  stream << "Done.\n";
  task.increment();
}

void BackupRunner::generate_backup_path()
{
  string random_hash = Crails::generate_random_string("abcdefg123456789", 8);

  remote_backup_path = "/tmp/puppeteer-backup-" + random_hash + ".tar.gz";
}

void BackupRunner::cleanup()
{
  ssh.exec("rm " + remote_backup_path, stream);
  ScriptRunner::cleanup();
}
