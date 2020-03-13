#ifndef  APPLICATION_MODEL_HPP
# define APPLICATION_MODEL_HPP

# include <sstream>
# include <memory>
# include <functional>

template<typename MODEL>
void fetch_one(unsigned long id, std::function<void (std::shared_ptr<MODEL>)> callback)
{
  auto model = std::make_shared<MODEL>(id);

  model->fetch().then([model, callback]() { callback(model); });
}

# define model_class(scope) \
  static std::string get_scope() { return scope; } \
  static std::string get_index_path() \
  { \
    return "#/" + get_scope(); \
  } \
  \
  std::string get_url() const \
  { \
    std::stringstream stream;\
    \
    stream << '/' << get_scope(); \
    if (get_id() != 0) \
      stream << '/' << get_id(); \
    return stream.str(); \
  } \
  \
  std::string get_path() const \
  { \
    std::stringstream stream; \
    \
    stream << "#/" << get_scope() << '/' << get_id(); \
    return stream.str(); \
  }

# define model_default_constructors(Type) \
  Type() {} \
  Type(unsigned long _id) { id = _id; }

#endif
