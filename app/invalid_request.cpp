#include "invalid_request.hpp"
#include <sstream>
#include <crails/request.hpp>

using namespace Crails;
using namespace std;

void on_invalid_request(Request& request, const InvalidRequest& e)
{
  Data response = request.params["response-data"];

  response["headers"]["Content-Type"] = "application/json";
  response["body"] = e.json;
  request.out.set_headers("Content-Type", "application/json");
  Server::SetResponse(
    request.params, request.out,
    Server::HttpCodes::bad_request,
    response["body"].as<std::string>()
  );
  request.params["response-data"]["code"] = Server::HttpCodes::bad_request;
  request.on_finished();
}

void MissingParameter::raise(const vector<string>& parameters)
{
  InvalidRequest request;
  stringstream stream;

  stream << '{'
         << "\"type\":\"missing parameter\"" << ','
         << "\"value\":[";
  for (auto it = parameters.begin() ; it != parameters.end() ; ++it)
  {
    if (it != parameters.begin())
      stream << ',';
    stream << '"' << *it << '"';
  }
  stream << "]}";
  request.json = stream.str();
  throw request;
}

void InvalidParameter::raise(const map<string,string>& parameters)
{
  InvalidRequest request;
  stringstream stream;

  stream << '{'
         << "\"type\":\"invalid parameter\"" << ','
         << "\"value\":[";
  for (auto it = parameters.begin() ; it != parameters.end() ; ++it)
  {
    if (it != parameters.begin())
      stream << ',';
    stream << "{\"" << it->first << "\":\"" << it->second << "\"}";
  }
  stream << "]}";
  request.json = stream.str();
  throw request;
}
