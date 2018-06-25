#ifndef  MODEL_BASE_HPP
# define MODEL_BASE_HPP

# include <crails/odb/model.hpp>

# pragma db object abstract
class Model : public ODB::Model
{
public:
  virtual void on_change() {}
};

#endif
