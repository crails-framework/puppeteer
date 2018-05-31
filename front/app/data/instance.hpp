#ifndef  INSTANCE_DATA_HPP
# define INSTANCE_DATA_HPP

# include <crails/front/mvc/model.hpp>

struct InstanceData : public Crails::Front::Model
{
public:
  unsigned long get_id() const { return id; }
  static std::string get_resource_name() { return "instance"; }
  std::string get_name() const { return name; }
  void set_name(std::string value) { name = value; }
  std::string get_path() const { return path; }
  void set_path(std::string value) { path = value; }
  std::string get_user() const { return user; }
  void set_user(std::string value) { user = value; }
  unsigned long get_machine_id() const { return machine_id; }
  void set_machine_id(unsigned long value) { machine_id = value; }
  unsigned long get_build_id() const { return build_id; }
  void set_build_id(unsigned long value) { build_id = value; }

  void serialize(OArchive& archive)
  {
    archive & id & name & path & user & machine_id & build_id;
  }

  void serialize(IArchive& archive)
  {
    archive & id & name & path & user & machine_id & build_id;
  }

protected:
  std::string name;
  std::string path;
  std::string user;
  unsigned long machine_id;
  unsigned long build_id;
  unsigned long id;
};

#endif
