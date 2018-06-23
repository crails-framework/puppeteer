#include "recipe_plugin.hpp"
#include "api.hpp"
#include "app/ssh/stream.hpp"

using namespace std;

void GandiPlugin::apply(const std::string& package, const std::string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  stream << '\n' << "## GANDI plugin\n";
  if (package == "setup")
    setup(recipe_folder, instance, variables, stream);
  else if (package == "uninstall")
    uninstall(recipe_folder, instance, variables, stream);
}

void GandiPlugin::setup(const string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  auto public_ip = variables.find("PUBLIC_IP");
  auto hostname  = variables.find("HOSTNAME");

  if (public_ip == variables.end() || hostname == variables.end())
    stream << "no PUBLIC_IP or HOSTNAME variable available for this instance\n";
  else if (domain_zone_id == 0 || domain_name == "")
    stream << "GandiPlugin is not configured\n";
  else
  {
    Gandi::Api api;
    auto pos = hostname->second.find('.' + domain_name);

    if (pos != std::string::npos)
    {
      string name = hostname->second.substr(0, pos);
      auto records = api.domain_zone_record_list(domain_zone_id);
      bool has_matching_record = false;
      bool needs_update = true;

      // Walking the record to check for an existing ZoneRecord
      stream << "matching subdomain `" << name << "` to `" << public_ip->second << "`\n";
      for (auto candidate : records)
      {
        if (candidate.get_name() == name)
        {
          has_matching_record = true;
          if (candidate.get_value() != public_ip->second)
          {
            candidate.set_value(public_ip->second);
            stream << "-> updating record " << candidate.get_id() << "\n";
          }
          else
            needs_update = false;
          break ;
        }
      }

      if (!needs_update)
        stream << "no changes needed\n";
      else
      {
        int new_zone_version;

        // Creating the new zone file version
        new_zone_version = api.domain_zone_version_new(domain_zone_id);

        // Performing the update/create (depending on whether a matching record has been found)
        if (has_matching_record)
          api.domain_zone_record_set(domain_zone_id, new_zone_version, records);
        else
        {
          Gandi::ZoneRecord record;

          record.set_name(name);
          record.set_value(public_ip->second);
          record.set_type("A");
          record.set_ttl(60 * 60 * 3);
          stream << "-> adding new record to domain zone\n";
          api.domain_zone_record_add(domain_zone_id, new_zone_version, record);
        }

        // Setting the new zone file version as the active zone file
        api.domain_zone_version_set(domain_zone_id, new_zone_version);
      }
    }
    else
      stream << "domain name ignored (`" << hostname->second << "` does not belong to fqdn `" << domain_name << "`)\n";
  }
}

void GandiPlugin::uninstall(const string& recipe_folder, Instance& instance, const map<string,string>& variables, Sync::Stream& stream) const
{
  auto hostname = variables.find("HOSTNAME");

  if (hostname == variables.end())
    stream << "no HOSTNAME variable available for this instance\n";
  else if (domain_zone_id == 0 || domain_name == "")
    stream << "GandiPlugin is not configured\n";
  else
  {
    Gandi::Api api;
    auto pos = hostname->second.find('.' + domain_name);

    if (pos != std::string::npos)
    {
      string name = hostname->second.substr(0, pos);
      auto records = api.domain_zone_record_list(domain_zone_id);
      bool has_matching_record = false;

      // Walking the record to check for an existing ZoneRecord
      for (auto it = records.begin() ; it != records.end() ; ++it)
      {
        if (it->get_name() == name)
        {
          records.erase(it);
          has_matching_record = true;
          break ;
        }
      }

      if (has_matching_record)
      {
        int new_zone_version = api.domain_zone_version_new(domain_zone_id);

        api.domain_zone_record_set(domain_zone_id, new_zone_version, records);
        api.domain_zone_version_set(domain_zone_id, new_zone_version);
      }
      else
        stream << "no record with name `" << name << "` found\n";
    }
    else
      stream << "domain name ignored (`" << hostname->second << "` does not belong to fqdn `" << domain_name << "`)\n";
  }
}
