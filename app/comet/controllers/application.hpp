#ifndef  FRONT_APPLICATION_CONTROLLER_HPP
# define FRONT_APPLICATION_CONTROLLER_HPP

# include <comet/mvc/controller.hpp>
# include "../application.hpp"
# include "app/comet/views/layouts/application.hpp"
# include <iostream>

class ApplicationController : public Comet::Controller
{
protected:
  ApplicationController(const Comet::Params& p) : Comet::Controller(p), router(Application::get().router)
  {
    std::cout << "Controller constructor" << std::endl;
    finalize_waiter = std::unique_ptr<Comet::Promise>(
      new Comet::Promise([this](std::function<void()> resolve, std::function<void()>)
      {
        on_done = resolve;
      })
    );
  }

  virtual ~ApplicationController()
  {
    std::cout << "Controller destroyed" << std::endl;
  }

  std::unique_ptr<Comet::Promise> finalize_waiter;
  std::function<void ()>          on_done;
  Comet::Router& router;

public:
  Comet::Promise initialize()
  {
    get_layout<ApplicationLayout>().reset_search_input();
    return Comet::Controller::initialize();
  }

  Comet::Promise finalize()
  {
    return *finalize_waiter;
  }

  Comet::Layout<ApplicationLayout>& use_application_layout()
  {
    return use_layout<ApplicationLayout>();
  }
};

#endif
