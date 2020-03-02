#include <crails/request_handlers/file.hpp>
#include <crails/request_handlers/action.hpp>
#include <crails/request_parser.hpp>
#include <crails/archive_parser.hpp>
#include <crails/request.hpp>
#include <crails/sentry.hpp>
#include <crails/getenv.hpp>

using namespace Crails;

const std::string Server::temporary_path = "/tmp";
extern thread_local Sentry sentry;

void Server::initialize_request_pipe()
{
  add_request_parser(new RequestDataParser);
  add_request_parser(new RequestArchiveParser);
  add_request_parser(new RequestFormParser);
  add_request_parser(new RequestJsonParser);
  add_request_parser(new RequestXmlParser);
  add_request_parser(new RequestMultipartParser);

  add_request_handler(new FileRequestHandler);
  add_request_handler(new ActionRequestHandler);

  if (Crails::getenv("SENTRY_ENABLED") == "true")
  {
    exception_catcher.add_exception_catcher<std::exception&>(
      [this](Request& request, const std::exception& e)
      {
        std::stringstream stream;

        sentry.capture_exception(request.params.as_data(), e);
        stream << boost_ext::trace(e);
        exception_catcher.default_exception_handler(request, (typeid(e).name()), e.what(), stream.str());
      }
    );
  }
}
