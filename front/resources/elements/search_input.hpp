#ifndef  FRONT_SEARCH_INPUT_HPP2
# define FRONT_SEARCH_INPUT_HPP2

# include "lib/cheerp-html/resources/elements/search_input.hpp"
# include "front/app/searches.hpp"

class SearchInput : public HtmlTemplate::SearchInput
{
public:
  inline void clear()
  {
    results = nullptr;
    signaler.trigger("results-changed");
  }

  void on_search_clicked();
  void reset();

  const std::vector<Puppeteer::Machine>&  get_machines() const;
  const std::vector<Puppeteer::Instance>& get_instances() const;
  const std::vector<Puppeteer::Build>&    get_builds() const;

private:
  void on_results_fetched(std::shared_ptr<SearchQuery>);
};

#endif
