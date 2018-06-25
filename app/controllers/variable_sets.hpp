#ifndef  VARIABLE_SET_CONTROLLER_HPP
# define VARIABLE_SET_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/variable_set.hpp"

class VariableSetController : public Crud::ArchiveController<VariableSet>
{
  typedef Crud::ArchiveController<VariableSet> Super;
  std::string get_view_path() const { return "variable_set_"; }
public:
  VariableSetController(Crails::Params& params) : Super(params)
  {}

  bool find_model()
  {
    if (!database.find_one(model))
      model = std::make_shared<VariableSet>();
    return true;
  }
};

#endif
