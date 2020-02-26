#ifndef  MODEL_CREDENTIAL_HPP
# define MODEL_CREDENTIAL_HPP

# include "model.hpp"
# include "lib/app/data/credential.hpp"

class OArchive;
class IArchive;

# pragma db object pointer(std::shared_ptr)
class Credential : public Model, public ModelData::Credential
{
  odb_instantiable()
public:
  #pragma db view pointer(std::shared_ptr) object(Credential)
  struct Count
  {
    #pragma db column("count(" + Credential::id + ")")
    size_t value;
  };

  std::string get_url()  const { return get_model_url(*this); }
  std::string get_path() const { return '#' + get_url(); }

  void serialize(OArchive&);
  void serialize(IArchive&);

  std::string get_jenkins_id() const;

# ifndef __CHEERP_CLIENT__
  bool can_destroy() const;
  void on_change();
  void before_destroy();
# endif
};

#endif
