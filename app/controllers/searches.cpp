#include "lib/odb/application-odb.hxx"
#include "searches.hpp"
#include <crails/front/archive.hpp>

using namespace std;
using namespace Crails;

SearchesController::SearchesController(Params& params) : Super(params)
{
}

template<typename MODEL>
static void append_search_results_to_archive(odb::result<MODEL>& results, OArchive& archive)
{
  unsigned short count = results.size();

  archive & count;
  for (auto& model : results)
    model.serialize(archive);
}

void SearchesController::search()
{
  OArchive              archive;
  string                search_term = params["query"];
  odb::result<Machine>  machines;
  odb::result<Instance> instances;
  odb::result<Build>    builds;

  database.find<Machine> (machines,  odb::query<Machine> ::name.like(search_term));
  database.find<Instance>(instances, odb::query<Instance>::name.like(search_term));
  database.find<Build>   (builds,    odb::query<Build>   ::name.like(search_term));

  append_search_results_to_archive<Machine> (machines,  archive);
  append_search_results_to_archive<Instance>(instances, archive);
  append_search_results_to_archive<Build>   (builds,    archive);
  render(archive);
}
