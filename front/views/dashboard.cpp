#include "dashboard.hpp"
#include "front/resources/elements/breadcrumbs.hpp"

using namespace std;
using namespace Crails::Front;

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
