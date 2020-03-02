#ifndef  FRONT_BREADCRUMBS_HPP
# define FRONT_BREADCRUMBS_HPP

# include "lib/cheerp-html/resources/elements/breadcrumbs.hpp"
# include <crails/odb/id_type.hpp>

class Breadcrumbs : public HtmlTemplate::Breadcrumbs
{
  static Breadcrumbs* instance;
public:
  Breadcrumbs();
  ~Breadcrumbs();

  static void reset();
  static void done();
  static void add_crumb(const std::string& label, const std::string& path);

  static void set_instance(ODB::id_type id);
  static void set_machine(ODB::id_type id);
  static void set_build(ODB::id_type id);
  static void set_recipe(ODB::id_type id);
  static void set_credential(ODB::id_type id);

  static void set_index_crumbs(const std::string& label, const std::string& path);

private:
  void set_crumb(unsigned short index, const std::string& label, const std::string& paht);

  template<typename MODEL>
  void set_model(ODB::id_type id, const std::string& label_singular, const std::string& label_plural)
  {
    set_crumb(1, label_plural, MODEL::get_index_path());
    if (id != ODB_NULL_ID)
    {
      MODEL model(id);
      set_crumb(2, label_singular, model.get_path());
    }
  }
};

#endif
