#ifndef  MY_CRUD_CONTROLLER_HPP
# define MY_CRUD_CONTROLLER_HPP

# include "modules/crud/controller.hpp"
# include <crails/archive.hpp>

namespace Crud
{
  template<typename MODEL, typename QUERY_OBJECT = MODEL>
  class ArchiveController : public Crud::Controller<MODEL, QUERY_OBJECT>
  {
    typedef Crud::Controller<MODEL, QUERY_OBJECT> Super;
  public:
    ArchiveController(Crails::Params& params) : Super(params) {}

    virtual void destroy()
    {
      if (Super::model->can_destroy())
        Super::destroy();
      else
        Super::respond_with(Super::ResponseStatus::bad_request);
    }

  protected:
    virtual bool edit_model(Data data)
    {
      Data archive_data = Super::params["archive_data"];

      if (archive_data.exists())
      {
        IArchive archive;

        archive.set_data(archive_data.as<std::string>());
        Super::model->serialize(archive);
        Super::model->on_change();
        return Super::validate();
      }
      return Super::edit_model(data);
    }
  };
}

#endif
