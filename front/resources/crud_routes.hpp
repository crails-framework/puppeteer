#ifndef  FRONT_CRUD_ROUTES_HPP
# define FRONT_CRUD_ROUTES_HPP

# include <crails/front/router.hpp>
# include "front/resources/modal.hpp"
# include "front/views/utility/main_view.hpp"
# include <boost/lexical_cast.hpp>

template<typename INDEX, typename SHOW, typename EDIT, typename DELETE>
void make_routes_for(Crails::Front::Router* router, const std::string& path)
{
  using namespace std;
  using namespace Crails::Front;

  router->match(path, [](const Params&)
  {
    auto view = std::make_shared<INDEX>();

    MainView::instance->attach(view);
    view->activate();
  });

  router->match(path + "/new", [](const Params&)
  {
    auto view = std::make_shared<EDIT>();

    MainView::instance->attach(view);
    view->activate();
  });

  router->match(path + "/:resource_id", [](const Params& params)
  {
    auto view = std::make_shared<SHOW>();
    auto id   = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(view);
    view->activate(id);
  });

  router->match(path + "/:resource_id/edit", [](const Params& params)
  {
    auto view = std::make_shared<EDIT>();
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));

    MainView::instance->attach(view);
    view->activate(id);
  });

  router->match(path + "/:resource_id/destroy", [router, path](const Params& params)
  {
    auto id = boost::lexical_cast<unsigned long>(params.at("resource_id"));
    auto modal = Modal<DELETE>::make("Removing");

    modal->el.activate(id);
    modal->open().then([router, path, modal, id]()
    {
      if (modal->ok())
      {
        modal->el.destroy([router, path](bool success)
        {
          if (success)
            router->navigate(path, true);
          else
            Crails::Front::window->alert("resource removal failed");
        });
      }
      else
        router->navigate(path + '/' + boost::lexical_cast<std::string>(id), false);
    });
  });
}

#endif
