#include "index.hpp"
#include "app/comet/views/resources/elements/breadcrumbs.hpp"

using namespace std;
using namespace Comet;

void Views::Dashboard::initialize_breadcrumbs()
{
  Breadcrumbs::reset();
  Breadcrumbs::done();
}

void Views::Dashboard::activate()
{
  initialize_breadcrumbs();
  last_builds.fetch_last_builds();
}
