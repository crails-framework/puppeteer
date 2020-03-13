#include "application_controller.hpp"
#include "app/invalid_request.hpp"
#include <crails/utils/timer.hpp>
#include <crails/archive.hpp>

using namespace std;

void ApplicationController::respond_with(Crails::Server::HttpCode code)
{
  response["status"] = (unsigned short)code;
}

void ApplicationController::render(const std::string& view)
{
  Utils::Timer timer;

  Crails::Controller::render(view);
  Crails::logger << Crails::Logger::Info
    << "Render " << view << " in " << timer.GetElapsedSeconds() << "s"
    << Crails::Logger::endl;
}

void ApplicationController::render(const OArchive& archive)
{
  response["headers"]["Content-Type"] = Archive::mimetype;
  response["body"] = archive.as_string();
}

void ApplicationController::require_parameters(const vector<string>& parameters) const
{
  auto missing_parameters = params.as_data().find_missing_keys(parameters);

  if (missing_parameters.size() > 0)
    MissingParameter::raise(missing_parameters);
}

