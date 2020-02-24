#include "deploy_runner.hpp"

using namespace std;

DeployRunner::DeployRunner(Ssh::Session& ssh, Recipe& recipe, Instance& instance, Sync::Task& task) : ScriptRunner(ssh, recipe, instance, task)
{
}

void DeployRunner::deploy_build(const string& build_id)
{
  variables["BUILD_TARBALL"] = get_remote_folder() + '/' + get_tarball_filename(build_id);
  upload_variables();
  upload_build(build_id);
  upload_script("deploy");
  run_script("deploy");
}

void DeployRunner::upload_build(const string& build_id)
{
  const std::string build_path  = instance.get_build()->get_build_path();
  const std::string tarball_src = build_path + '/' + get_tarball_filename(build_id);
  auto scp = ssh.make_scp_session(get_remote_folder(), SSH_SCP_WRITE);

  stream << "Uploading build tarball" << tarball_src << '\n';
  scp->push_file(tarball_src, get_tarball_filename(build_id));
  stream << "Done.\n";
  task.increment();
}
