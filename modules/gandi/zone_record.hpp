#ifndef  GANDI_ZONE_RECORD_HPP
# define GANDI_ZONE_RECORD_HPP

# include <crails/xmlrpc/struct.hpp>

namespace Gandi
{
  class ZoneRecord : public XmlRpc::Struct
  {
  public:
    ZoneRecord(const XmlRpc::Variable& variable) : Struct(variable) {}
    ZoneRecord() {}

    XmlRpc_string_property(name)
    XmlRpc_string_property(type)
    XmlRpc_string_property(id)
    XmlRpc_string_property(value)
    XmlRpc_int_property(ttl)
  };
}

#endif
