#ifndef  FRONT_SEARCHES_HPP
# define FRONT_SEARCHES_HPP

# include "app/comet/models/machine.hpp"
# include "app/comet/models/instance.hpp"
# include "app/comet/models/build.hpp"

class SearchQuery
{
public:
  typedef std::function<void (std::shared_ptr<SearchQuery>)> Callback;

  static void find(const std::string& query, Callback callback);

  const std::vector<Front::Machine>&  get_machines()  const { return machines; }
  const std::vector<Front::Instance>& get_instances() const { return instances; }
  const std::vector<Front::Build>&    get_builds()    const { return builds; }

private:
  void parse(const std::string&);

  std::vector<Front::Machine>  machines;
  std::vector<Front::Instance> instances;
  std::vector<Front::Build>    builds;
};

#endif
