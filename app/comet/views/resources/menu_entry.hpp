#ifndef  STRUCT_MENU_ENTRY_HPP
# define STRUCT_MENU_ENTRY_HPP

# include <string>

struct MenuEntry
{
  MenuEntry(const std::string& a, const std::string& b, const std::string& c) : icon(a), label(b), route(c) {}

  std::string icon;
  std::string label;
  std::string route;
};

#endif
