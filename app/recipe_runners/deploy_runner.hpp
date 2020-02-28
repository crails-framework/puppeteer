#ifndef  RECIPE_DEPLOY_RUNNER_HPP
# define RECIPE_DEPLOY_RUNNER_HPP

# include "script_runner.hpp"

struct DeployRunner : public ScriptRunner
{
  DeployRunner(Ssh::Session&, Recipe&, Instance&, Sync::Task&);

  std::string get_tarball_filename(const std::string& build_id) const
  {
    return build_id + ".tar.gz";
  }

  void deploy_build(const std::string& build_id);
  void upload_build(const std::string& build_id);

  void cleanup();
private:
  std::string remote_tarball_path;
};

#endif
