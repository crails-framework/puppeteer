#include <crails/databases.hpp>
#include <crails/database_url.hpp>
#include <crails/getenv.hpp>

using namespace Crails;

static const DatabaseUrl production(getenv("PUPPETEER_DATABASE_URL"));
extern const std::string pgu = "postgres";
extern const std::string pgp = Crails::getenv("EDP_PGP");

const Databases::Settings Databases::settings = {
  {
    "production", {
      {
        "odb", {
          { "type",     "pgsql" },
          { "host",     production.hostname.c_str() },
          { "name",     production.database_name.c_str() },
          { "user",     production.username.c_str() },
          { "password", production.password.c_str() },
          { "port",     production.port }
        }
      }
    }
  },

  {
    "development", {
      {
        "odb", {
          { "type", "pgsql" },
          { "host", "127.0.0.1" },
          { "name", "puppeteer_dev" },
          { "user", "postgres" },
          { "password", "" },
          { "port", (unsigned int)5432 }
        }
      }
    }
  },

  {
    "test", {
      {
        "odb", {
          { "type", "pgsql" },
          { "host", "127.0.0.1" },
          { "name", "puppeteer_test" },
          { "user", "postgres" },
          { "password", "" },
          { "port", (unsigned int)5432 }
        }
      }
    }
  }
};
