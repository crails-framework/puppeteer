#ifndef  MODEL_BASE_HPP
# define MODEL_BASE_HPP

# include <crails/odb/id_type.hpp>
# include <string>
# include <sstream>

class ApplicationModel
{
protected:
  template<typename MODEL_TYPE>
  static std::string get_model_url(const MODEL_TYPE& model)
  {
    std::stringstream stream;

    stream << '/' << MODEL_TYPE::scope;
    if (model.get_id() != ODB_NULL_ID)
      stream << '/' << model.get_id();
    return stream.str();
  }
};

# ifndef __CHEERP_CLIENT__
#  include <crails/odb/model.hpp>

#  pragma db object abstract
class Model : public ODB::Model, public ApplicationModel
{
public:
  virtual void on_change() {}
};
# else
#  define odb_instantiable()
#  define odb_instantiable_impl(param)
typedef ApplicationModel Model;
# endif

#endif
