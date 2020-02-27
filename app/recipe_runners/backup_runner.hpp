#ifndef  RECIPE_BACKUP_RUNNER_HPP
# define RECIPE_BACKUP_RUNNER_HPP

# include "script_runner.hpp"
# include "app/models/backup.hpp"

struct BackupRunner : public ScriptRunner
{
  static const std::string backup_script_name;
  std::string remote_backup_path;
  Backup& backup;

  BackupRunner(Ssh::Session& s, Backup& b, Instance& i, Recipe& r, Sync::Task& t);

  void upload_backup_script(Backup::Action action);
  void run_backup_script();
  void push_tarball(const std::string& input_file);
  void pull_tarball(const std::string& output_file);

  void cleanup();

private:
  void generate_backup_path();
};

#endif
