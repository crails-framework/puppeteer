#include "searches.hpp"
#include <crails/front/http.hpp>
#include <crails/front/archive.hpp>

using namespace std;
using namespace Crails::Front;

void SearchQuery::find(const string& query, Callback callback)
{
  auto request = Http::Request::get("/search");

  request->send().then([request, callback]()
  {
    auto response = request->get_response();

    if (response->ok())
    {
      auto search_query = make_shared<SearchQuery>();

      search_query->parse(response->get_response_text());
      callback(search_query);
    }
  });
}

template<typename MODEL>
static void append_search_results_to_array(std::vector<MODEL>& array, IArchive& archive)
{
  unsigned short count;

  archive & count;
  array.resize(count);
  for (unsigned short i = 0 ; i < count ; ++i)
    array[i].serialize(archive);
}

void SearchQuery::parse(const string& archive_text)
{
  IArchive archive;

  archive.set_data(archive_text);
  append_search_results_to_array<Puppeteer::Machine> (machines,  archive);
  append_search_results_to_array<Puppeteer::Instance>(instances, archive);
  append_search_results_to_array<Puppeteer::Build>   (builds,    archive);
}
