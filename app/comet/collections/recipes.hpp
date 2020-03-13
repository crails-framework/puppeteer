#ifndef  RECIPES_COLLECTION_HPP
# define RECIPES_COLLECTION_HPP

# include "app/comet/models/recipe.hpp"
# include <crails/comet/mvc/collection.hpp>

namespace Front
{
  class Recipes : public Comet::ArchiveCollection<Recipe>
  {
  public:
    std::string get_url() const
    {
      return '/' + Recipe::get_scope();
    }
  };
}

#endif
