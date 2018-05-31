#ifndef  BUILD_DATA_HPP
# define BUILD_DATA_HPP

# include <crails/front/mvc/model.hpp>

struct BuildData : public Crails::Front::Model
{
public:
  unsigned long get_id() const { return id; }
  static std::string get_resource_name() { return "build"; }
  std::string get_name() const { return name; }
  void set_name(std::string value) { name = value; }
  std::string get_git() const { return git; }
  void set_git(std::string value) { git = value; }
  std::string get_branch() const { return branch; }
  void set_branch(std::string value) { branch = value; }
  std::string get_options() const { return options; }
  void set_options(std::string value) { options = value; }
  unsigned long get_recipe_id() const { return recipe_id; }
  void set_recipe_id(unsigned long value) { recipe_id = value; }

  void serialize(OArchive& archive)
  {
    archive & id & name & git & branch & options & recipe_id;
  }

  void serialize(IArchive& archive)
  {
    archive & id & name & git & branch & options & recipe_id;
  }

protected:
  std::string name;
  std::string git;
  std::string branch;
  std::string options;
  unsigned long recipe_id;
  unsigned long id;
};

#endif
