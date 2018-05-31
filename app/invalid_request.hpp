#ifndef  INVALID_REQUEST_HPP
# define INVALID_REQUEST_HPP

# include <string>
# include <vector>
# include <map>

namespace Crails { class Request; }

struct InvalidRequest
{
  std::string json;
};

struct MissingParameter
{
  static void raise(const std::vector<std::string>& parameters);
};

struct InvalidParameter
{
  static void raise(const std::map<std::string, std::string>& parameters);
};

void on_invalid_request(Crails::Request&, const InvalidRequest&);

#endif
