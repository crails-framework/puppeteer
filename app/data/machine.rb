Model.add "Machine", ["Machine", "app/models/machine.hpp"] do
  resource_name "machine"
  order_by 'name'

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'ip'
end
