#include "credential.hpp"
#ifndef __CHEERP_CLIENT__
# include "app/jenkins/jenkins.hpp"
# include "lib/odb/application-odb.hxx"
#endif

using namespace std;
using namespace Crails;

odb_instantiable_impl(Credential)

std::string Credential::get_jenkins_id() const
{
  return "puppeteer_" + name;
}

#ifndef __CHEERP_CLIENT__
void Credential::on_change()
{
  Jenkins jenkins;
  int status = jenkins.create_credentials(get_jenkins_id(), username, password);

  if (status >= 400)
    throw std::runtime_error("unable to sync credentials with jenkins");
}

void Credential::before_destroy()
{
  Jenkins jenkins;
  int status = jenkins.delete_credentials(get_jenkins_id());

  if (status >= 400)
    throw std::runtime_error("unable to destroy jenkins credential " + get_jenkins_id());
}

void log_destroy_failure_on_bounded_resources(const std::string& resource, unsigned long count, const std::string& bound_resources)
{
  logger << Logger::Info;
  logger << "Cannot destroy " << resource << ": ";
  logger << count << ' ' << bound_resources << " are still bounded to it" << Logger::endl;
}

bool Credential::can_destroy() const
{
  auto& database = *(ODB::Connection::instance);
  unsigned long bound_recipe_count, bound_build_count;

  bound_recipe_count = database.count<Recipe>(odb::query<Recipe>::credential_id == get_id());
  bound_build_count  = database.count<Build>(odb::query<Build>::credential_id == get_id());
  if (bound_recipe_count)
    log_destroy_failure_on_bounded_resources("credentials `" + get_name() + '`', bound_recipe_count, "recipes");
  if (bound_build_count)
    log_destroy_failure_on_bounded_resources("credentials `" + get_name() + '`', bound_build_count,  "builds");
  return bound_recipe_count + bound_build_count == 0;
}
#endif
