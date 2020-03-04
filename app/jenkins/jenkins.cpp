#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "jenkins.hpp"
#include "config/jenkins.hpp"
#include <sstream>
#include <crails/http.hpp>
#include <crails/server.hpp>
#include <crails/utils/string.hpp>

using namespace std;

typedef boost::network::http::client_options<boost::network::http::client::tag_type>
  client_options;

Jenkins::Jenkins() : client(client_options().io_service(Crails::Server::get_io_service()))
{
}

string Jenkins::get_url() const
{
  stringstream stream;

  stream << protocol << "://"
         << user << ':' << api_token << '@'
         << hostname << ':' << port;
  return stream.str();
}

string Jenkins::puppeteer_folder_url() const
{
  return get_url() + "/job/" + puppeteer_folder;
}

string Jenkins::create_job_url(const string& jobname) const
{
  return puppeteer_folder_url() + "/createItem?name=" + Crails::Http::Url::Encode(jobname);
}

string Jenkins::update_job_url(const string& jobname) const
{
  return job_url(jobname) + "/config.xml";
}

string Jenkins::job_url(const string& jobname) const
{
  return puppeteer_folder_url() + "/job/" + jobname;
}

string Jenkins::build_url(const string& jobname, const string& build_id) const
{
  return job_url(jobname) + '/' + build_id;
}

void Jenkins::prepare_query(boost::network::http::client::request& request)
{
  string authorization = "Basic ";

  authorization += Crails::base64_encode(user + ':' + api_token);
  request << boost::network::header("Authorization", authorization);
}

bool Jenkins::job_exists(const std::string& jobname)
{
  boost::network::http::client::request request(job_url(jobname));
  prepare_query(request);
  auto response = client.get(request);
  auto status = boost::network::http::status(response);

  return !(status == 404);
}

#define TO_STRING_MACRO(VAR) #VAR;

DataTree Jenkins::get_last_builds()
{
  const string limit = TO_STRING_MACRO(LAST_BUILDS_LIST_SIZE);
  const string url   = puppeteer_folder_url() + "/api/json";
  const string param = "jobs[name,url,builds[number,result,duration,url,timestamp]{0," + limit + "}]";
  DataTree result;

  boost::network::http::client::request request(
    url + "?tree=" + Crails::Http::Url::Encode(param)
  );

  prepare_query(request);
  auto response = client.get(request);
  auto status = boost::network::http::status(response);

  if (status == 200)
  {
    string body = boost::network::http::body(response);

    boost::replace_all(body, "http://jenkins", public_url);
    result.from_json(body);
  }
  return result;
}

DataTree Jenkins::get_project_data(const std::string& jobname)
{
  const string url = job_url(jobname) + "/api/json";
  boost::network::http::client::request request(url);
  DataTree result;

  prepare_query(request);
  auto response = client.get(request);
  auto status = boost::network::http::status(response);

  if (status == 200)
  {
    string body = boost::network::http::body(response);

    boost::replace_all(body, "http://jenkins", public_url);
    result.from_json(body);
  }
  return result;
}

int Jenkins::push_config(const std::string& jobname, const std::string& config)
{
  string url = job_exists(jobname) ? update_job_url(jobname) : create_job_url(jobname);
  boost::network::http::client::request request(url);

  prepare_query(request);
  request << boost::network::header("Connection", "close");
  request << boost::network::header("Content-Type", "application/xml");
  request << boost::network::header("Content-Length", boost::lexical_cast<string>(config.length()));
  request << boost::network::body(config);

  std::cout << "[Jenkins] Pushing config " << jobname << ':' << std::endl << config << std::endl << std::endl;

  auto response = client.post(request);
  return boost::network::http::status(response);
}

int Jenkins::delete_build(const string& jobname, unsigned int build_id)
{
  return delete_build(jobname, boost::lexical_cast<string>(build_id));
}

int Jenkins::delete_build(const string& jobname, const string& build_id)
{
  string url = build_url(jobname, build_id) + "/doDelete";
  boost::network::http::client::request request(url);
  prepare_query(request);
  auto response = client.post(request);

  return boost::network::http::status(response);
}

int Jenkins::delete_job(const string& jobname)
{
  string url = update_job_url(jobname) + "/doDelete";
  boost::network::http::client::request request(url);
  prepare_query(request);
  auto response = client.post(request);

  return boost::network::http::status(response);
}

int Jenkins::enable_job(const string& jobname)
{
  string url = job_url(jobname) + "/enable";
  boost::network::http::client::request request(url);
  prepare_query(request);
  auto response = client.post(request);

  return boost::network::http::status(response);
}

int Jenkins::disable_job(const string& jobname)
{
  string url = job_url(jobname) + "/disable";
  boost::network::http::client::request request(url);
  prepare_query(request);
  auto response = client.post(request);

  return boost::network::http::status(response);
}

int Jenkins::delete_credentials(const string& id)
{
  string url = get_url()
             + "/credentials/store/system/domain/_/credential/"
             + Crails::Http::Url::Encode(id) + "/doDelete";
  boost::network::http::client::request request(url);

  prepare_query(request);
  auto response = client.post(request);
  return boost::network::http::status(response);
}

int Jenkins::create_credentials(const string& id, const string& username, const string& password)
{
  string url = get_url() + "/credentials/store/system/domain/_/createCredentials";
  stringstream body_stream;
  string body;
 
  body_stream << "{\n"
    << "  \"\": \"0\",\n"
    << "  \"credentials\": {\n"
    << "    \"scope\": \"GLOBAL\",\n"
    << "    \"id\": \"" << id << "\",\n"
    << "    \"username\": \"" << username << "\",\n"
    << "    \"password\": \"" << password << "\",\n"
    << "    \"description\": \"puppeteer managed credential\",\n"
    << "    \"$class\": \"com.cloudbees.plugins.credentials.impl.UsernamePasswordCredentialsImpl\"\n"
    << "  }\n"
    << '}';

  boost::network::http::client::request request(url);

  body = "json=" + body_stream.str();
  //body = Crails::Http::Url::Encode(body);
  prepare_query(request);
  request << boost::network::header("Connection", "close");
  request << boost::network::header("Content-Type", "application/x-www-form-urlencoded");
  request << boost::network::header("Content-Length", boost::lexical_cast<string>(body.length()));
  request << boost::network::body(body);

  auto response = client.post(request);
  int status = boost::network::http::status(response);

  if (status >= 400)
  {
    string resp = boost::network::http::body(response);
    std::cout << "jenkins refused to add credentials: " << resp << std::endl;
  }
  return status;
}
