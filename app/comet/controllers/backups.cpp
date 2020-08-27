#include "backups.hpp"
#include "app/comet/collections/backups.hpp"
#include "app/comet/views/backups/new.hpp"
#include "app/comet/views/backups/show.hpp"
#include <comet/lexical_cast.hpp>
#include <comet/http.hpp>

using namespace std;
using namespace Comet;

BackupsController::BackupsController(const Params& p) : ApplicationController(p)
{
  instance_id = lexical_cast<unsigned long>(params.at("id"));
}

void BackupsController::show()
{
  find_model().then([this]()
  {
    if (model)
    {
      auto view = std::make_shared<Views::BackupShow>();

      use_application_layout().render(view);
      view->activate(model);
    }
    else
    {
      auto view = std::make_shared<Views::BackupNew>();

      view->set_instance_id(instance_id);
      use_application_layout().render(view);
      view->activate();
    }
    on_done();
  });
}

void BackupsController::update()
{
  find_model().then([this]()
  {
    if (model)
    {
      auto view = std::make_shared<Views::BackupNew>();

      use_application_layout().render(view);
      view->activate(model);
    }
    else
    {
      std::cout << "ERROR: no backup settings found for this instance" << std::endl;
    }
    on_done();
  });
}

Promise BackupsController::find_model()
{
  const auto url     = Front::Backup::get_url_for_instance(instance_id);
  auto       request = Http::Request::get(url);

  request->set_headers({
    {"Accept", Archive::mimetype}
  });
  return request->send().then([this, request]()
  {
    auto response = request->get_response();

    if (response->ok() && response->has_body())
    {
      model = make_shared<Front::Backup>();
      model->parse(response->get_response_text());
    }
  });
}
