#ifndef  APP_JENKINS_HPP
# define APP_JENKINS_HPP

# include <string>
# include <crails/datatree.hpp>
# include "config/http_client.hpp"

class Jenkins
{
  static const std::string    protocol;
  static const unsigned short port;
  static const std::string    hostname;
  static const std::string    puppeteer_folder;
  static const std::string    user;
  static const std::string    api_token;
  static const std::string    public_url;
public:
  Jenkins();

  std::string update_job_url(const std::string& jobname) const;
  std::string create_job_url(const std::string& jobname) const;
  std::string job_url(const std::string& jobname) const;
  std::string build_url(const std::string& jobname, const std::string& build_id) const;
  std::string puppeteer_folder_url() const;
  std::string get_url() const;

  bool     job_exists(const std::string& jobname);
  int      push_config(const std::string& jobname, const std::string& config);
  int      delete_job(const std::string& jobname);
  int      enable_job(const std::string& jobname);
  int      disable_job(const std::string& jobname);
  int      delete_build(const std::string& jobname, unsigned int build_id);
  int      delete_build(const std::string& jobname, const std::string& build_id);
  DataTree get_project_data(const std::string& jobname);
  DataTree get_last_builds();

  int create_credentials(const std::string& id, const std::string& username, const std::string& password);
  int delete_credentials(const std::string& id);

private:
  void prepare_query(boost::network::http::client::request&);

  boost::network::http::client client;
};

#endif
