#include "lib/odb/application-odb.hxx"
#include "app/ssh/session.hpp"
#include "machines.hpp"
#include <crails/sidekic.hpp>

using namespace std;
using namespace Crails;

MachineController::MachineController(Params& params) : Super(params)
{
}

void MachineController::fetch_state()
{
  require_model();
  if (model)
  {
    OArchive     archive;
    stringstream os_stream;
    stringstream hdd_stream;
    stringstream ram_stream;

    model->open_ssh([&](Ssh::Session& ssh)
    {
      Sync::Stream ssh_os_stream(os_stream);
      Sync::Stream ssh_hdd_stream(hdd_stream);
      Sync::Stream ssh_ram_stream(ram_stream);

      // Get OS version
      ssh.exec("lsb_release -a 2> /dev/null | grep 'Distributor ID:' | cut -d: -f2", ssh_os_stream);
      ssh.exec("lsb_release -a 2> /dev/null | grep 'Release:' | cut -d: -f2", ssh_os_stream);
      // Get space left on root partition
      ssh.exec("df -h | grep \"/$\" | tr -s ' ' | cut -d' ' -f3", ssh_hdd_stream);
      hdd_stream << " / ";
      ssh.exec("df -h | grep \"/$\" | tr -s ' ' | cut -d' ' -f2", ssh_hdd_stream);
      // Get memory
      ssh.exec("free -h | grep 'Mem:' | cut -d: -f2", ssh_ram_stream);
    });
    archive & os_stream.str() & hdd_stream.str() & ram_stream.str();
    render(archive);
  }
}

void MachineController::upgrade()
{
  require_model();
  if (model)
  {
    DataTree task_params;
    string   task_uid;

    task_params["machine_id"] = model->get_id();
    task_params["tasks"]      = params["tasks"].defaults_to<string>("all");
    task_uid = Sidekic::async_task("machine_upgrade", task_params);
    Crails::Controller::render(TEXT, task_uid);
  }
}
