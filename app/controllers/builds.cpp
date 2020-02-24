#include "lib/odb/application-odb.hxx"
#include "builds.hpp"
#include "app/jenkins/jenkins.hpp"
#include <crails/utils/string.hpp>

using namespace std;
using namespace Crails;

std::vector<std::string> list_directory(const std::string& directory_path);

BuildController::BuildController(Params& params) : Super(params)
{
}

void BuildController::after_create()
{
  model->on_change();
}

void BuildController::enable()
{
  require_model();
  if (model)
  {
    Jenkins jenkins;
    int status = jenkins.enable_job(model->get_name());

    if (status >= 400)
      response["status"] = 500;
  }
}

void BuildController::disable()
{
  require_model();
  if (model)
  {
    Jenkins jenkins;
    int status = jenkins.disable_job(model->get_name());

    if (status >= 400)
      response["status"] = 500;
  }
}

void BuildController::builds()
{
  require_model();
  if (model)
  {
    Jenkins  jenkins;
    DataTree data = jenkins.get_project_data(model->get_name());

    response["body"] = data.as_data().to_json();
    model->update_last_build(data);
    database.save(*model);
  }
}

bool string_ends_with(std::string const &fullString, std::string const &ending)
{
  if (fullString.length() >= ending.length())
  {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  return false;
}

void BuildController::available_builds()
{
  require_model();
  if (model)
  {
    auto files = list_directory(model->get_build_path());
    stringstream json;
    vector<string> filenames;

    for (const auto& filepath : files)
    {
      if (string_ends_with(filepath, ".tar.gz"))
      {
        string filename = *(split(filepath, '/').rbegin());

        filenames.push_back(filename.substr(0, filename.length() - 7));
      }
    }
    std::sort(filenames.begin(), filenames.end(), [](const std::string& a, const std::string& b) -> bool
    {
      stringstream str_a; stringstream str_b;
      unsigned int int_a, int_b;

      str_a << a;     str_b << b;
      str_a >> int_a; str_b >> int_b;
      return int_a > int_b;
    });
    json << '[';
    for (auto it = filenames.begin() ; it != filenames.end() ; ++it)
    {
      if (it != filenames.begin())
        json << ',';
      json << '"' << (*it) << '"';
    }
    json << ']';
    response["body"] = json.str();
  }
}
