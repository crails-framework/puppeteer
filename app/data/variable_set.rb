Model.add "VariableSet", hpp: "app/models/variable_set.hpp" do
  resource_name "variable_set"

  visibility :protected
  property 'std::string',    'value'
  property 'unsigned short', 'owner_type', default: 0
  property 'ODB::id_type',   'owner_id'
end
