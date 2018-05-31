#ifndef  RECIPE_DATA_HPP
# define RECIPE_DATA_HPP

# include <crails/front/mvc/model.hpp>

struct RecipeData : public Crails::Front::Model
{
public:
  unsigned long get_id() const { return id; }
  static std::string get_resource_name() { return "recipe"; }
  std::string get_name() const { return name; }
  void set_name(std::string value) { name = value; }

  void serialize(OArchive& archive)
  {
    archive & id & name;
  }

  void serialize(IArchive& archive)
  {
    archive & id & name;
  }

protected:
  std::string name;
  unsigned long id;
};

#endif
