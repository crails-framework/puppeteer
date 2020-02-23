#ifndef  RECIPE_SCRIPT_RUNNER_HPP
# define RECIPE_SCRIPT_RUNNER_HPP

# include <map>
# include <crails/sync/task.hpp>
# include "app/models/recipe.hpp"
# include "app/models/instance.hpp"
# include "app/ssh/session.hpp"

struct ScriptRunner
{
  typedef std::map<std::string, std::string> Variables;

  Ssh::Session& ssh;
  Recipe&       recipe;
  Instance&     instance;
  Sync::Task&   task;
  Sync::Stream  stream;
  Variables     variables;

  ScriptRunner(Ssh::Session&, Recipe&, Instance&, Sync::Task&); 

  virtual std::string get_remote_folder() const
  {
    return Recipe::remote_path + '/' + instance.get_name();
  }

  virtual std::string get_variable_filepath() const
  {
    return get_remote_folder() + "/variables";
  }

  virtual std::string get_script_path(const std::string& script_name) const
  {
    return recipe.get_repository_path() + '/' + script_name + ".sh";
  }

  virtual void upload_variables();
  virtual void upload_script(const std::string& script_name);
  virtual int run_script(const std::string& script_name);
};

#endif
