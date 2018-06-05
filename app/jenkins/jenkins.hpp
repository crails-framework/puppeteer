#ifndef  APP_JENKINS_HPP
# define APP_JENKINS_HPP

# include <string>
# include "config/http_client.hpp"

class Jenkins
{
  static const std::string    protocol;
  static const unsigned short port;
  static const std::string    hostname;
  static const std::string    puppeteer_folder;
  static const std::string    user;
  static const std::string    api_token;
public:
  Jenkins();

  std::string update_job_url(const std::string& jobname) const;
  std::string create_job_url(const std::string& jobname) const;
  std::string job_url(const std::string& jobname) const;
  std::string puppeteer_folder_url() const;
  std::string get_url() const;

  bool job_exists(const std::string& jobname);
  int  push_config(const std::string& jobname, const std::string& config);
  int  delete_job(const std::string& jobname);

  int create_credentials(const std::string& id, const std::string& username, const std::string& password);

private:
  void prepare_query(boost::network::http::client::request&);

  boost::network::http::client client;
};

#endif
