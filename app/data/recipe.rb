Model.add "Recipe", ["Recipe", "app/models/recipe.hpp"] do
  resource_name "recipe"
  order_by 'name'

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'git_url'
  property 'std::string', 'git_branch'
end
