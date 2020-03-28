#include "machine.hpp"
#ifndef __CHEERP_CLIENT__
# include "lib/odb/application-odb.hxx"
# include "app/ssh/session.hpp"
#endif

odb_instantiable_impl(Machine)

#ifndef __CHEERP_CLIENT__
void Machine::open_ssh(std::function<void (Ssh::Session&)> callback) const
{
  Ssh::Session ssh;

  ssh.should_accept_unknown_hosts(true);
  ssh.connect(Recipe::remote_user, get_ip());
  ssh.authentify_with_pubkey();
  callback(ssh);
}

void log_destroy_failure_on_bounded_resources(const std::string& resource, unsigned long count, const std::string& bound_resources);

bool Machine::can_destroy() const
{
  auto& database = *(ODB::Connection::instance);
  unsigned long bound_instance_count;

  bound_instance_count = database.count<Instance>(odb::query<Instance>::machine_id == get_id());
  if (bound_instance_count)
    log_destroy_failure_on_bounded_resources("machine `" + get_name() + '`', bound_instance_count, "instances");
  return bound_instance_count == 0;
}

void Machine::before_destroy()
{
}
#endif
