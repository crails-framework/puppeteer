#include <boost/lexical_cast.hpp>
#include "jenkins.hpp"
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

int Jenkins::push_config(const std::string& jobname, const std::string& config)
{
  string url = job_exists(jobname) ? update_job_url(jobname) : create_job_url(jobname);
  boost::network::http::client::request request(url);

  prepare_query(request);
  request << boost::network::header("Connection", "close");
  request << boost::network::header("Content-Type", "application/xml");
  request << boost::network::header("Content-Length", boost::lexical_cast<string>(config.length()));
  request << boost::network::body(config);

  auto response = client.post(request);
  return boost::network::http::status(response);
}

int Jenkins::delete_job(const string& jobname)
{
  string url = update_job_url(jobname) + "/doDelete";
  boost::network::http::client::request request(url);
  auto response = client.post(request);

  return boost::network::http::status(response);
}
