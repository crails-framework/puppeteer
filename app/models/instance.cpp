#include "instance.hpp"
#include "lib/odb/application-odb.hxx"
#include "app/ssh/session.hpp"
#include "app/ssh/channel.hpp"

odb_instantiable_impl(Instance)

void Instance::install()
{
  Ssh::Session session;

  session.should_accept_unknown_hosts(true);
  session.connect(get_user(), get_machine()->get_ip());
  session.authentify_with_pubkey();
  {
    auto channel1 = session.make_channel();
    auto channel2 = session.make_channel();

    auto status = channel1->exec("ls -l", std::cout);
    std::cout << "remote command returned with status: " << status << std::endl;

    status = channel2->exec("pwd", std::cout);
    std::cout << "can we run several commands: " << status << std::endl;
  }
}
