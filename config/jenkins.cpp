#include "app/jenkins/jenkins.hpp"
#include <crails/getenv.hpp>

using namespace std;

const string         Jenkins::protocol         = Crails::getenv("JENKINS_PROTOCOL", "http");
const string         Jenkins::hostname         = Crails::getenv("JENKINS_HOST");
const unsigned short Jenkins::port             = Crails::getenv_as("JENKINS_PORT", 80);
const string         Jenkins::puppeteer_folder = Crails::getenv("JENKINS_PUPPETEER_FOLDER", "Puppeteer");

const string         Jenkins::user             = Crails::getenv("JENKINS_USER");
const string         Jenkins::api_token        = Crails::getenv("JENKINS_API_TOKEN");
