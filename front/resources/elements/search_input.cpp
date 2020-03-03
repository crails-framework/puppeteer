#include "search_input.hpp"
#include "front/app/searches.hpp"

using namespace std;

void SearchInput::reset()
{
  set_results(nullptr);
  signaler.trigger("results-changed");
  input.value("");
}

bool SearchInput::monitor_query()
{
  const string query = input.get_value();

  if (last_query != query)
  {
    if (query.length() > 0)
      on_search_clicked();
    else
      reset();
    last_query = query;
  }
  return true;
}

void SearchInput::on_search_clicked()
{
  const string query = input.get_value();

  SearchQuery::find(input.get_value(), std::bind(&SearchInput::on_results_fetched, this, std::placeholders::_1));
}

void SearchInput::on_results_fetched(shared_ptr<SearchQuery> _results)
{
  results = _results;
  signaler.trigger("results-changed");
}

const vector<Puppeteer::Machine>& SearchInput::get_machines() const
{
  static const vector<Puppeteer::Machine> empty_array;

  if (results)
    return results->get_machines();
  return empty_array;
}

const vector<Puppeteer::Instance>& SearchInput::get_instances() const
{
  static const vector<Puppeteer::Instance> empty_array;

  if (results)
    return results->get_instances();
  return empty_array;
}

const vector<Puppeteer::Build>& SearchInput::get_builds() const
{
  static const vector<Puppeteer::Build> empty_array;

  if (results)
    return results->get_builds();
  return empty_array;
}
