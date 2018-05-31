Model.add "Recipe", ["Recipe", "app/models/recipe.hpp"] do
  resource_name "recipe"

  visibility :protected
  property 'std::string', 'name'
end
