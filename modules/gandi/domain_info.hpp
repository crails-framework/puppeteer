#ifndef  GANDI_DOMAIN_INFO_HPP
# define GANDI_DOMAIN_INFO_HPP

# include <crails/xmlrpc/struct.hpp>
# include "contacts.hpp"

namespace Gandi
{
  class DomainInfo : public XmlRpc::Struct
  {
  public:
    DomainInfo(const XmlRpc::Variable& variable) : Struct(variable) {}

    XmlRpc_string_property(entity_id)
    XmlRpc_date_property(date_updated)
    XmlRpc_date_property(date_delete)
    XmlRpc_bool_property(tld_lock)
    XmlRpc_bool_property(is_prenium)
    XmlRpc_bool_property(is_special_price)
    XmlRpc_date_property(date_hold_begin)
    XmlRpc_date_property(date_registry_end)
    XmlRpc_date_property(authinfo_expiration_date)
    XmlRpc_string_array_property(nameservers)
    XmlRpc_date_property(date_restore_end)
    XmlRpc_int_property(id)
    XmlRpc_string_property(authinfo)
    XmlRpc_date_property(date_hold_end)
    XmlRpc_string_array_property(services)
    XmlRpc_date_property(date_pending_delete_end)
    XmlRpc_date_property(date_renew_begin)
    XmlRpc_string_property(fqdn)
    XmlRpc_date_property(date_registry_creation)
    XmlRpc_string_property(tld)
    XmlRpc_date_property(date_created)

    Contacts get_contacts() const
    {
      return Contacts(data["contacts"]);
    }

    // get_status, type array<?>
    // get_tags,   type array<?>
    // get_zone_id, type ?
    // get_autorenew, type ?
  };
}

#endif
