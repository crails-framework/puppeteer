#ifndef  GANDI_API_HPP
# define GANDI_API_HPP

# include <crails/datatree.hpp>
# include <boost/lexical_cast.hpp>

# include "config/http_client.hpp" // BOOST HTTPS define must be defined before xmlrpc/client.hpp gets included
# include <crails/xmlrpc/client.hpp>

# include "domain_info.hpp"
# include "zone.hpp"
# include "zone_record.hpp"

namespace Gandi
{
  class Api : public XmlRpc::Client
  {
    static const std::string endpoint;
    static const std::string key;
    std::string api_version;
  public:
    Api();

    const std::string& version_info();

    // Domain API
    std::string                        domain_available(const std::string& domain_name);
    std::map<std::string, std::string> domain_available(const std::vector<std::string>&);

    DomainInfo              domain_info(const std::string& domain_name);
    std::vector<DomainInfo> domain_list();
    std::vector<Zone>       domain_zone_list();
    void                    domain_zone_set(const std::string& domain_name, int id);
    int                     domain_zone_version_new(int id);
    void                    domain_zone_version_set(int id, int version);
    std::vector<ZoneRecord> domain_zone_record_list(int id, int version = 0);
    void                    domain_zone_record_add(int zone_id, int version, const ZoneRecord& record);
    void                    domain_zone_record_set(int zone_id, int version, const std::vector<ZoneRecord>& records);
  };
}

#endif
