#include "credential.hpp"
#ifndef __CHEERP_CLIENT__
# include "app/jenkins/jenkins.hpp"
# include "lib/odb/application-odb.hxx"
#endif

using namespace std;

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
#endif
