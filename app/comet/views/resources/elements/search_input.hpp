#ifndef  FRONT_SEARCH_INPUT_HPP2
# define FRONT_SEARCH_INPUT_HPP2

# include "html/views/resources/elements/search_input.hpp"
# include "app/comet/searches.hpp"

class SearchInput : public HtmlTemplate::SearchInput
{
public:
  virtual ~SearchInput() {}

  inline void clear()
  {
    results = nullptr;
    signaler.trigger("results-changed");
  }

  void on_search_clicked();
  void reset();

  const std::vector<Front::Machine>&  get_machines() const;
  const std::vector<Front::Instance>& get_instances() const;
  const std::vector<Front::Build>&    get_builds() const;

private:
  bool monitor_query();
  void on_results_fetched(std::shared_ptr<SearchQuery>);

  std::string last_query;
};

#endif
