$: << (File.dirname(__FILE__) + "/..")
require 'cppgen/model'
require 'cppgen/generator_base'

ARRAY_SEP = ','

class ImportGenerator < GeneratorBase
  def generate_for object
    return if object[:classname].nil?
    @klassname = object[:classname]
    @klassimp = "#{@klassname.split("::").join("")}Csv"

    @import_properties_src = ""
    @import_hasone_src = ""
    @import_hasmany_src = ""
    @export_header_src = ""
    @export_properties_src = ""
    self.instance_eval &object[:block]

    src = %{
struct #{@klassimp}
{
  typedef std::shared_ptr<#{@klassname}>  EntryPtr;
  typedef std::map<Db::id_type, EntryPtr> EntryMap;

  void import(const Csv::Document& document)
  {
    typedef std::pair<Db::id_type, EntryPtr> MapItem;
    auto db = Db::Connection::instance;

    for (const auto& row : document)
    {
      auto id = boost::lexical_cast<Db::id_type>(row.get("id"));
      auto model = std::make_shared<#{@klassname}>();

#{@import_properties_src}
      db->save(*model);
      entries.insert(MapItem(id, model));
    }
  }
#{@import_hasone_src}
#{@import_hasmany_src}
  void export_to(Csv::Writer& writer, std::vector<#{@klassname}> array)
  {
    Csv::Row header;

    header.push_back("id");
#{@export_header_src}    writer.insert(header);
    for (auto& model : array)
    {
      Csv::Row row;

      row.push_back(boost::lexical_cast<std::string>(model.get_id()));
#{@export_properties_src}      writer.insert(row);
    }
  }

  EntryMap entries;
  bool ignore_missing_references = false;
};
    }
  end

  def property type, name, options = {}
    # IMPORT
    src = "      if (document.has_column(\"#{name}\"))\n"
    src += "        "
    src += "model->set_#{name}("
    if type == "std::string" || type == "LocaleString"
      src += "row.get(\"#{name}\")"
    elsif type == "bool"
      src += "row.get(\"#{name}\") == \"t\""
    elsif type == "DataTree"
      src += "DataTree().from_json(row.get(\"#{name}\"))"
    else
      src += "boost::lexical_cast<#{type}>(row.get(\"#{name}\"))"
    end
    src += ");"
    @import_properties_src += src + "\n"

    # EXPORT
    src = "header.push_back(\"#{name}\");"
    @export_header_src += "    " + src + "\n"
    if type == "std::string"
      src = "row.push_back(model.get_#{name}());"
    elsif type == "bool"
      src = "row.push_back(model.get_#{name}() ? \"t\" : \"f\");"
    elsif type == "DataTree" || type == "LocaleString"
      src = "row.push_back(model.get_#{name}().to_json());"
    else
      src = "row.push_back(boost::lexical_cast<std::string>(model.get_#{name}()));"
    end
    @export_properties_src += "      " + src + "\n"
  end

  def has_one type, name, options = {}
    # IMPORT
    @import_hasone_src += %{
  void import_#{name}(const Csv::Document& document, std::map<Db::id_type, std::shared_ptr<#{type}> > entries)
  {
    if (document.has_column("#{name}"))
    {
      for (const auto& row : document)
      {
        auto id = boost::lexical_cast<Db::id_type>(row.get("id"));
        auto rel_id = boost::lexical_cast<Db::id_type>(row.get("#{name}"));
        auto rel  = entries[rel_id];

        if (rel_id != 0 && rel == nullptr)
        {
          std::stringstream stream;
          stream << "entry " << id << " references unexisting #{name}[" << rel_id << ']';
          if (ignore_missing_references)
            Crails::logger << stream.str() << Crails::Logger::endl;
          else
            throw std::runtime_error(stream.str());
        }
        else if (rel_id != 0)
        {
          auto database = Db::Connection::instance;
          auto self = this->entries[id];
          self->set_#{name}(rel);
          database->save(*self);
        }
      }
    }
  }
    }

    # EXPORT
    src = "header.push_back(\"#{name}\");"
    @export_header_src += "    " + src + "\n"
    src = "row.push_back(boost::lexical_cast<std::string>(model.get_#{name}_id()));"
    @export_properties_src += "      " + src + "\n"
  end

  def has_many type, name, options = {}
    # IMPORT
    singular_name = get_singular_name name
    @import_hasmany_src += %{
  void import_#{name}(const Csv::Document& document, std::map<Db::id_type, std::shared_ptr<#{type}> > entries)
  {
    if (document.has_column("#{singular_name}_ids"))
    {
      for (const auto& row : document)
      {
        auto id = boost::lexical_cast<Db::id_type>(row.get("id"));
        auto rel_ids = Crails::split(row.get("#{singular_name}_ids"), '#{ARRAY_SEP}');

        if (rel_ids.size() > 0)
        {
          auto database = Db::Connection::instance;
          auto self = this->entries[id];

          for (auto rel_id : rel_ids)
          {
            auto rel = entries[boost::lexical_cast<Db::id_type>(rel_id)];

	    if (rel != nullptr)
              self->add_#{singular_name}(rel);
	    else
	    {
              std::stringstream stream;
              stream << "entry " << id << " references unexisting #{singular_name}[" << rel_id << ']';
              if (ignore_missing_references)
                Crails::logger << stream.str() << Crails::Logger::endl;
              else
                throw std::runtime_error(stream.str());
	    }
          }
          database->save(*self);
        }
      }
    }
  }
    }

    # EXPORT
    src = "header.push_back(\"#{singular_name}_ids\");"
    @export_header_src += "    " + src + "\n"
    src = %{      {
        std::stringstream stream;
        auto items = model.get_#{name}();
        for (auto item : items)
          stream << item->get_id() << '#{ARRAY_SEP}';
        row.push_back(stream.str());
      }
}
    @export_properties_src += src
  end

  class << self
    def extension ; ".csv.hpp" ; end

    def make_file filename, data
      file_define = "_#{filename[0...-3].upcase.gsub "/", "_"}_CSV_HPP"
      source = <<CPP
#ifndef  #{file_define}
# define #{file_define}

# include <map>
# include <string>
# include <csv/document.hpp>
# include <csv/writer.hpp>
# include <boost/lexical_cast.hpp>
# include <crails/utils/string.hpp>
# include "modules/odb/connection.hpp"

#{data[:headers].join ""}

#{data[:bodies].join "\n"}
#endif
CPP
    end
  end
end
