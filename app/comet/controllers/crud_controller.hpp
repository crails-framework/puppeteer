#ifndef  FRONT_CRUD_CONTROLLER_HPP
# define FRONT_CRUD_CONTROLLER_HPP

# include "application.hpp"
# include "app/comet/views/resources/modal.hpp"
# include <comet/lexical_cast.hpp>
# include <comet/mvc/collection.hpp>
# include <comet/globals.hpp>

# define match_resource(ROUTE, CONTROLLER) \
  match_route(ROUTE, CONTROLLER, index); \
  match_route(ROUTE + "/new", CONTROLLER, create); \
  match_route(ROUTE + "/:resource_id", CONTROLLER, show); \
  match_route(ROUTE + "/:resource_id/edit", CONTROLLER, update); \
  match_route(ROUTE + "/:resource_id/destroy", CONTROLLER, destroy);

template<typename DESC>
class CrudController : public ApplicationController
{
  typedef typename DESC::Collection CollectionType;
public:
  CrudController(const Comet::Params& p) : ApplicationController(p)
  {
  }

  void index()
  {
    std::shared_ptr<typename DESC::IndexView> view;

    collection = std::make_shared<CollectionType>();
    view = std::make_shared<typename DESC::IndexView>(collection);
    collection->fetch().then([this, view]()
    {
      use_application_layout().render(view);
      view->activate();
      on_done();
    });
  }

  void show()
  {
    auto view = std::make_shared<typename DESC::ShowView>();

    find_model().then([this, view]()
    {
      use_application_layout().render(view);
      view->activate(model);
      on_done();
    });
  }

  void create()
  {
    auto view = std::make_shared<typename DESC::CreateView>();

    use_application_layout().render(view);
    view->activate();
    on_done();
  }

  void update()
  {
    auto view = std::make_shared<typename DESC::EditView>();

    find_model().then([this, view]()
    {
      use_application_layout().render(view);
      view->activate(model);
      on_done();
    });
  }

  void destroy()
  {
    find_model().then([this]()
    {
      auto modal = Modal<typename DESC::DestroyView>::make("Removing");

      modal->el.activate(model);
      modal->open().then([this, modal]()
      {
        if (modal->ok())
        {
          modal->el.destroy([this](bool success)
          {
            if (success)
              router.navigate(DESC::resource_path, true);
            else
              Comet::window->alert("resource removal failed");
          });
        }
        else
          router.navigate(DESC::resource_path + '/' + Comet::lexical_cast<std::string>(model->get_id()), false);
        on_done();
      });
    });
  }

private:
  Comet::Promise find_model()
  {
    auto id = Comet::lexical_cast<unsigned long>(params.at("resource_id"));

    model = std::make_shared<typename CollectionType::Model>(id);
    return model->fetch();
  }

  std::shared_ptr<CollectionType>                 collection;
  std::shared_ptr<typename CollectionType::Model> model;
};

#endif
