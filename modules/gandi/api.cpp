#include "api.hpp"
#include <crails/getenv.hpp>

#ifndef SERVER_DEBUG
# define GANDI_API_ENDPOINT "https://rpc.gandi.net/xmlrpc/"
#else
# define GANDI_API_ENDPOINT "https://rpc.ote.gandi.net/xmlrpc/"
#endif

using namespace std;
using namespace Gandi;

const string Gandi::Api::endpoint = Crails::getenv("GANDI_API_ENDPOINT", GANDI_API_ENDPOINT);
const string Gandi::Api::key      = Crails::getenv("GANDI_API_KEY");

Api::Api() : XmlRpc::Client(Gandi::Api::endpoint)
{
}

const string& Api::version_info()
{
  if (!api_version.length())
  {
    auto response_params = call("version.info", Gandi::Api::key);
    auto variable = response_params[0];

    api_version = variable["api_version"].as_string();
  }
  return api_version;
}

map<string, string> Api::domain_available(const vector<string>& domain_names)
{
  auto params = call("domain.available", Gandi::Api::key, domain_names);
  map<string, string> result;

  for (auto member : params[0].members)
    result.emplace(member.first, member.second.as_string());
  return result;
}

std::string Api::domain_available(const string& domain_name)
{
  vector<string> domain_names = { domain_name };

  return domain_available(domain_names).at(domain_name);
}

DomainInfo Api::domain_info(const string& domain_name)
{
  auto params = call("domain.info", Gandi::Api::key, domain_name);

  return DomainInfo(params[0]);
}

vector<DomainInfo> Api::domain_list()
{
  vector<DomainInfo> list;
  auto params = call("domain.list", Gandi::Api::key);

  list.reserve(params[0].array.size());
  for (auto variable : params[0].array)
    list.push_back(DomainInfo(variable));
  return list;
}

vector<Zone> Api::domain_zone_list()
{
  vector<Zone> list;
  auto params = call("domain.zone.list", Gandi::Api::key);

  list.reserve(params[0].array.size());
  for (auto variable : params[0].array)
    list.push_back(Zone(variable));
  return list;
}

vector<ZoneRecord> Api::domain_zone_record_list(int id, int version)
{
  vector<ZoneRecord> list;
  auto params = call("domain.zone.record.list", Gandi::Api::key, id, version);

  list.reserve(params[0].array.size());
  for (auto variable : params[0].array)
    list.push_back(ZoneRecord(variable));
  return list;
}

void Api::domain_zone_record_add(int zone_id, int version, const ZoneRecord& record)
{
  call("domain.zone.record.add", Gandi::Api::key, zone_id, version, record);
}

void Api::domain_zone_record_set(int zone_id, int version, const vector<ZoneRecord>& records)
{
  call("domain.zone.record.set", Gandi::Api::key, zone_id, version, records);
}

int Api::domain_zone_version_new(int id)
{
  auto params = call("domain.zone.version.new", Gandi::Api::key, id);

  return params[0].as_int();
}

void Api::domain_zone_version_set(int id, int version)
{
  call("domain.zone.version.set", Gandi::Api::key, id, version);
}

void Api::domain_zone_set(const string& domain_name, int id)
{
  call("domain.zone.set", Gandi::Api::key, domain_name, id);
}
