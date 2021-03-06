#include "backup.hpp"
#include <comet/http.hpp>

using namespace std;

string Front::Backup::get_url_for_instance(ODB::id_type id)
{
  stringstream stream;

  stream << "/instances/" << id << "/backup";
  return stream.str();
}

string Front::Backup::get_path_for_instance(ODB::id_type id)
{
  return '#' + get_url_for_instance(id);
}

void Front::Backup::fetch_for_instance(ODB::id_type id, function<void (shared_ptr<Backup>)> callback)
{
  const auto url     = get_url_for_instance(id);
  auto       request = Comet::Http::Request::get(url);

  request->set_headers({
    {"Accept", Archive::mimetype}
  });
  request->send().then([request, callback]()
  {
    shared_ptr<Backup> model;
    auto response = request->get_response();

    if (response->ok() && response->has_body())
    {
      model = make_shared<Backup>();
      model->parse(response->get_response_text());
    }
    callback(model);
  });
}
