#ifndef  HTML_FA_ICON_HPP
# define HTML_FA_ICON_HPP

# include <crails/front/custom_element.hpp>

class FaIcon : public Crails::Front::CustomElement
{
public:
  FaIcon() : Crails::Front::CustomElement("i")
  {
    attr("class", get_class_names());
    bound_attributes.push_back(Crails::Front::Bindable(*this, "class", [this]() -> std::string
    {
      return get_class_names();
    }).use_mode(Crails::Front::Bindable::SignalBind, "class-changed"));
    bind_attributes();
  }

  void set_name(const std::string& value)
  {
    name = value;
    signaler.trigger("class-changed");
  }

  const std::string& get_name() const
  {
    return name;
  }

  void set_suffix(const std::string& value)
  {
    suffix = value;
    signaler.trigger("class-changed");
  }

  const std::string& get_suffix() const
  {
    return suffix;
  }

private:
  std::string get_class_names()
  {
    return "fa fa-" + name + ' ' + suffix;
  }

  std::string name = "cirlce";
  std::string suffix;
};

#endif
