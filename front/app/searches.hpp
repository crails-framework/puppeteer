#ifndef  FRONT_SEARCHES_HPP
# define FRONT_SEARCHES_HPP

# include "machines.hpp"
# include "instances.hpp"
# include "builds.hpp"

class SearchQuery
{
public:
  typedef std::function<void (std::shared_ptr<SearchQuery>)> Callback;

  static void find(const std::string& query, Callback callback);

  const std::vector<Puppeteer::Machine>&  get_machines()  const { return machines; }
  const std::vector<Puppeteer::Instance>& get_instances() const { return instances; }
  const std::vector<Puppeteer::Build>&    get_builds()    const { return builds; }

private:
  void parse(const std::string&);

  std::vector<Puppeteer::Machine>  machines;
  std::vector<Puppeteer::Instance> instances;
  std::vector<Puppeteer::Build>    builds;
};

#endif
