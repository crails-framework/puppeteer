#ifndef  VARIABLE_LIST_HPP
# define VARIABLE_LIST_HPP

# include <string>
# include <map>
# include <crails/utils/string.hpp>

class VariableList : public std::string
{
public:
  VariableList(const std::string& str) : std::string(str)
  {}

  VariableList()
  {}

  std::map<std::string, std::string> to_map() const
  {
    std::map<std::string, std::string> map;

    to_map(map);
    return map;
  }

  void to_map(std::map<std::string, std::string>& map) const
  {
    std::list<std::string> parts = Crails::split(*this, ';');

    for (auto part : parts)
    {
      std::list<std::string> subparts = Crails::split(part, '=');

      if (subparts.size() == 2)
        map[*subparts.begin()] = *subparts.rbegin();
      else
      {
#ifndef __CHEERP_CLIENT__
        throw std::runtime_error("invalid variable list string");
#else
        __asm__("throw 'invalid variable list string';");
#endif
      }
    }
  }

  void from_map(std::map<std::string, std::string> map)
  {
    std::string::operator=("");
    for (auto entry : map)
    {
      if (length() > 0)
        operator+=(';');
      operator+=(entry.first + '=' + entry.second);
    }
  }
};

#endif
