add_include "app/models/credential.hpp", include_in_header: true

Model.add "Recipe", hpp: "app/models/recipe.hpp" do
  resource_name "recipe"
  order_by 'name'

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'git_url'
  property 'std::string', 'git_branch'
  property 'std::string', 'last_tip'
  has_one "Credential", "credential", joined: false
end
