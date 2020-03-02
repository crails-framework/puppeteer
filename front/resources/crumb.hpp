#ifndef  BREADCRUMBS_CRUMB_HPP
# define BREADCRUMBS_CRUMB_HPP

struct Crumb
{
  Crumb(const std::string& a, const std::string& b) : label(a), path(b) {}
  Crumb() {}

  std::string label;
  std::string path;
};

#endif
