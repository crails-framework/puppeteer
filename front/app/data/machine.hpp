#ifndef  MACHINE_DATA_HPP
# define MACHINE_DATA_HPP

# include <crails/front/mvc/model.hpp>

struct MachineData : public Crails::Front::Model
{
public:
  unsigned long get_id() const { return id; }
  static std::string get_resource_name() { return "machine"; }
  std::string get_name() const { return name; }
  void set_name(std::string value) { name = value; }
  std::string get_ip() const { return ip; }
  void set_ip(std::string value) { ip = value; }

  void serialize(OArchive& archive)
  {
    archive & id & name & ip;
  }

  void serialize(IArchive& archive)
  {
    archive & id & name & ip;
  }

protected:
  std::string name;
  std::string ip;
  unsigned long id;
};

#endif
