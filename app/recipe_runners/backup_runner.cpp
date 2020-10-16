#include "backup_runner.hpp"
#include <crails/utils/string.hpp>
#include <sstream>

using namespace std;

const string BackupRunner::backup_script_name = "backup";

BackupRunner::BackupRunner(Ssh::Session& s, Backup& b, Instance& i, Recipe& r, Sync::Task& t)
  : ScriptRunner(s, r, i, t), backup(b)
{
}

std::string BackupRunner::get_backup_tmp_folder()
{
  return get_remote_folder() + "/output";
}

void BackupRunner::upload_backup_script(Backup::Action action)
{
  require_remote_folder();
  auto scp = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);
  stringstream script;

  script << "#!/bin/bash" << std::endl << std::endl;
  script << "source " << get_variable_filepath() << std::endl;
  script << "export BACKUP_DIR=\"" << get_backup_tmp_folder() << "\"" << std::endl;
  script << "mkdir -p \"$BACKUP_DIR\"" << std::endl;
  script << "chmod 777 \"$BACKUP_DIR\"" << std::endl;
  switch (action)
  {
  case Backup::BackupAction:
    generate_backup_path();
    script << backup.get_backup_script() << std::endl;
    script << "cd \"$BACKUP_DIR\"" << std::endl;
    script << "tar -czf " << remote_backup_path << " *" << std::endl;
    script << "rm -Rf \"" << get_remote_folder() << "\"" << std::endl;
    break ;
  case Backup::RestoreAction:
    script << backup.get_restore_script();
    break ;
  }

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
  remote_backup_path = get_remote_folder() + "/restore.tar.gz";
  {
    auto scp = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);

    stream << "Uploading " << source << "...\n";
    scp->push_file(source, "restore.tar.gz");
  }
  stream << "Extracting " << source << "...\n";
  ssh.exec("cd \"" + get_backup_tmp_folder() + "\" && tar -xf \"" + remote_backup_path + '"', stream);
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
  ssh.exec("rm -Rf \"" + get_backup_tmp_folder() + "\"", stream);
  ScriptRunner::cleanup();
}
