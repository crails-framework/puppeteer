Model.add "Credential", hpp: "app/models/credential.hpp" do
  resource_name "credential"
  order_by "name"

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'username'
  property 'std::string', 'password'
end
