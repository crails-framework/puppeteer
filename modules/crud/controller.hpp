#ifndef  CRUD_CONTROLLER_HPP
# define CRUD_CONTROLLER_HPP

# include <crails/safe_ptr.hpp>
# include "app/controllers/application_controller.hpp"
# include "paginator.hpp"
# include "app/models/helpers.hpp"

# define crudify(router, path, controller) \
  router. SetRoute("GET",    std::string(path),          controller, index) \
  router. SetRoute("GET",    std::string(path) + "/:id", controller, show) \
  router. SetRoute("POST",   std::string(path),          controller, create) \
  router. SetRoute("PATCH",  std::string(path) + "/:id", controller, update) \
  router. SetRoute("PUT",    std::string(path) + "/:id", controller, update) \
  router. SetRoute("DELETE", std::string(path) + "/:id", controller, destroy)

namespace Crud
{
  template<typename MODEL, typename QUERY_OBJECT = MODEL>
  class Controller : public ApplicationController
  {
  public:
    typedef odb::query<QUERY_OBJECT> Query;

    Controller(Crails::Params& params) :
      ApplicationController(params),
      paginator(params.as_data())
    {
    }

    void initialize()
    {
      ApplicationController::initialize();
      initialize_crud();
    }

    void initialize_crud()
    {
      for (auto action : get_actions_with_model())
      {
        if (action == params["controller-data"]["action"].template as<std::string>())
        {
          require_model();
          break ;
        }
      }
    }

    void index()
    {
      find_models();
      paginator.decorate_view(vars, [this]() {
        return database.count<QUERY_OBJECT>(get_index_query());
      });
      vars["models"] = &models;
      render(get_index_view());
    }

    void show()
    {
      vars["model"] = model.get();
      render(get_show_view());
    }

    void create()
    {
      Data model_params = get_model_params();

      if (initialize_model(model_params) && edit_model(model_params))
      {
        database.save(*model);
        vars["model"] = model.get();
        render(get_show_view());
        after_create();
	std::cout << "end of crud create method" << std::endl;
      }
    }

    void update()
    {
      if (edit_model(get_model_params()))
      {
        vars["model"] = model.get();
        render(get_show_view());
        database.save(*model);
        after_update();
      }
    }

    void destroy()
    {
      database.destroy(*model);
      after_destroy();
    }

  protected:
    virtual void after_update() {}
    virtual void after_create() {}
    virtual void after_destroy() {}

    virtual std::string get_view_path() const
    {
      if (MODEL::scope != "")
        return MODEL::scope + "/";
      return "";
    }

    virtual std::string get_show_view()  const { return get_view_path() + "show"; }
    virtual std::string get_index_view() const { return get_view_path() + "index"; }

    virtual std::vector<std::string> get_actions_with_model() const
    {
      return { "show", "update", "destroy" };
    }

    virtual bool initialize_model(Data data)
    {
      model = std::make_shared<MODEL>();
      return true;
    }

    virtual bool edit_model(Data data)
    {
      model->edit(data);
      model->on_change();
      return validate();
    }

    virtual bool validate()
    {
      if (!model->is_valid())
      {
        Crails::Controller::render(Crails::Controller::JSON,
          model->errors.as_data()
        );
        response["status"] = ResponseStatus::bad_request;
        return false;
      }
      return true;
    }

    virtual Data get_model_params()
    {
      if (MODEL::scope == "")
        return params.as_data();
      return params[MODEL::scope];
    }

    void require_model()
    {
      if (!find_model())
        response["status"] = ResponseStatus::not_found;
    }

    virtual bool find_model()
    {
      auto param_name = get_id_param_name();
      auto id = params[param_name].template as<ODB::id_type>();

      return database.find_one(model, odb::query<MODEL>::id == id);
    }

    virtual bool find_models()
    {
      Query query = get_index_query();
      odb::result<QUERY_OBJECT> results;

      paginator.decorate_query(query);
      database.find(results, query);
      models = odb::to_vector<MODEL, QUERY_OBJECT>(results);
      return models.size() > 0;
    }

    virtual std::string get_id_param_name() const
    {
      return "id";
    }

    virtual Query get_index_query() const
    {
      return Query(true);
    }

    Paginator          paginator;
    safe_ptr<MODEL>    model;
    std::vector<MODEL> models;
  };
}

#endif
