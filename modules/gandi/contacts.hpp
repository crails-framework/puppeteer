#ifndef  GANDI_CONTACTS_HPP
# define GANDI_CONTACTS_HPP

# include <crails/xmlrpc/struct.hpp>

namespace Gandi
{
  class Contact : public XmlRpc::Struct
  {
  public:
    Contact(const XmlRpc::Variable& variable) : Struct(variable) {}

    XmlRpc_int_property(id)
    XmlRpc_string_property(handle)
  };

  class Contacts
  {
    Contact owner, admin, bill, tech, reseller;
  public:
    Contacts(const XmlRpc::Variable& variable) : 
      owner(variable["owner"]),
      admin(variable["admin"]),
      bill(variable["bill"]),
      tech(variable["tech"]),
      reseller(variable["reseller"])
    {
    }

    const Contact& get_owner()    { return owner; }
    const Contact& get_admin()    { return admin; }
    const Contact& get_bill()     { return bill; }
    const Contact& get_tech()     { return tech; }
    const Contact& get_reseller() { return reseller; }
  };
}

#endif
