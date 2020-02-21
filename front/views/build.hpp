#ifndef  VIEW_BUILD_HPP
# define VIEW_BUILD_HPP

# include "utility/model_view.hpp"
# include "lib/cheerp-html/views/build.hpp"
# include "../app/builds.hpp"

# include <crails/front/globals.hpp>

namespace Views
{
  class Build : public ModelView<Puppeteer::Build, HtmlTemplate::Build>
  {
  public:
    Build()
    {
    }

    void on_model_received()
    {
      refresh_builds();
    }

    void set_enabled_state(bool value)
    {
      // TODO re-implement
    }

    void refresh_builds(client::Event* = 0)
    {
      Crails::Front::Ajax::query("GET", model->get_url() + "/builds").callbacks({
        std::bind(&Build::on_builds_fetched,      this, std::placeholders::_1),
        std::bind(&Build::on_builds_fetch_failed, this, std::placeholders::_1)
      })();
    }

    std::vector<Crails::Front::Object>& get_builds() { return builds; }

    void on_builds_fetched(const Crails::Front::Ajax& ajax)
    {
      auto data = ajax.get_response_as_json();

      if (!data.is_undefined())
      {
        std::cout << "fetched builds" << std::endl;
        std::string buildable = data["buildable"];

	set_enabled_state(buildable == "true");
	builds = data["builds"];
	signaler.trigger("builds-fetched");
      }
      else
        std::cout << "fetched builds, but there's no json response" << std::endl;
    }

    void on_builds_fetch_failed(const Crails::Front::Ajax&)
    {
      std::cout << "failed to fetch builds" << std::endl;
    }

  private:
    std::vector<Crails::Front::Object> builds;
  };
}

#endif
