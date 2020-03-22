#ifndef  HTML_FA_ICON_HPP
# define HTML_FA_ICON_HPP

# include <comet/custom_element.hpp>

class FaIcon : public Comet::CustomElement
{
public:
  FaIcon() : Comet::CustomElement("i")
  {
    attr("class", get_class_names());
    bound_attributes.push_back(Comet::Bindable(*this, "class", [this]() -> std::string
    {
      return get_class_names();
    }).use_mode(Comet::Bindable::SignalBind, "class-changed"));
    bind_attributes();
  }

  void set_name(const std::string& value)
  {
    name = value;
    signaler.trigger("class-changed");
  }

  inline const std::string& get_name() const
  {
    return name;
  }

  void set_spinning(bool value)
  {
    spinning = value;
    signaler.trigger("class-changed");
  }

  inline bool get_spinning() const { return spinning; }

  void set_suffix(const std::string& value)
  {
    suffix = value;
    signaler.trigger("class-changed");
  }

  inline const std::string& get_suffix() const
  {
    return suffix;
  }

private:
  std::string get_class_names()
  {
    std::string result = "fa fa-" + get_name() + ' ' + get_suffix();

    if (get_spinning())
      result += " fa-spin";
    return result;
  }

  std::string name = "cirlce";
  std::string suffix;
  bool        spinning = false;
};

#endif
