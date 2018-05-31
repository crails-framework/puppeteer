add_include "app/models/instance.hpp"
add_include "app/models/recipe.hpp", true

Model.add "Build", ["Build", "app/models/build.hpp"] do
  resource_name "build"

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'git'
  property 'std::string', 'branch'
  property 'std::string', 'options'
  has_one "Recipe", "recipe", joined: false, dependent: :destroy
end
