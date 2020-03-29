Model.add "Machine", hpp: "app/models/machine.hpp" do
  resource_name "machine"
  order_by 'name'

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'ip'
  property 'std::time_t', 'last_upgrade', default: 0
end
