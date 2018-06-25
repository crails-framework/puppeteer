#include "variable_set.hpp"
#include "variable_list.hpp"
#include "lib/odb/application-odb.hxx"

using namespace std;

odb_instantiable_impl(VariableSet)

void VariableSet::collect_variables(map<string,string>& variables) const
{
  const VariableList local_variables = get_value();

  local_variables.to_map(variables);
}

void VariableSet::collect_global_variables(map<string,string>& variables)
{
  auto& database = *ODB::Connection::instance;
  odb::result<VariableSet> variable_sets;

  database.find<VariableSet>(variable_sets, odb::query<VariableSet>::owner_type == 0);
  for (const auto& variable_set : variable_sets)
    variable_set.collect_variables(variables);
}
