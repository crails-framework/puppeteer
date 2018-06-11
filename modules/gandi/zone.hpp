#ifndef  GANDI_ZONE_HPP
# define GANDI_ZONE_HPP

# include <crails/xmlrpc/struct.hpp>

namespace Gandi
{
  class Zone : public XmlRpc::Struct
  {
  public:
    Zone(const XmlRpc::Variable& variable) : Struct(variable) {}
    Zone() {}

    XmlRpc_date_property(date_updated)
    XmlRpc_int_property(id)
    XmlRpc_int_property(version)
    XmlRpc_bool_property(public)
    XmlRpc_string_property(name)
  };
}

#endif
