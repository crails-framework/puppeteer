#ifndef  BUILDS_CONTROLLER_HPP
# define BUILDS_CONTROLLER_HPP

# include "crud_controller.hpp"
# include "app/models/build.hpp"

class BuildController : public Crud::ArchiveController<Build>
{
  typedef Crud::ArchiveController<Build> Super;
  std::string get_view_path() const { return "build_"; }
public:
  BuildController(Crails::Params&);

  void enable();
  void disable();
  void builds();

protected:
  void after_create();
};

#endif
