#ifndef  RECIPE_HPP
# define RECIPE_HPP

# include "modules/odb/model.hpp"
# include "lib/app/data/recipe.hpp"

class OArchive;
class IArchive;

# pragma db object pointer(std::shared_ptr)
class Recipe : public Db::Model, public ModelData::Recipe
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Recipe)
  struct Count
  {
    #pragma db column("count(" + Recipe::id + ")")
    size_t value;
  };

  void serialize(OArchive&);
  void serialize(IArchive&);
};

#endif
