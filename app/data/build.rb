add_include "app/models/instance.hpp"
add_include "app/models/recipe.hpp", true
add_include "app/models/credential.hpp", true

Model.add "Build", ["Build", "app/models/build.hpp"] do
  resource_name "build"
  order_by 'name'

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'git'
  property 'std::string', 'branch'
  property 'std::string', 'options'
  property 'std::string', 'variables'
  property 'std::time_t', 'last_build'
  has_one "Recipe",     "recipe",     joined: false, dependent: :destroy
  has_one "Credential", "credential", joined: false
end
