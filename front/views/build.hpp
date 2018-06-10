#ifndef  VIEW_BUILD_HPP
# define VIEW_BUILD_HPP

# include "utility/model_view.hpp"
# include "utility/theme.hpp"
# include "../app/builds.hpp"

# include <crails/front/globals.hpp>

namespace Views
{
  class Build : public ModelView<Puppeteer::Build>
  {
    Button refresh_builds_button;
    Button enable_button;
    bool   is_enabled = false;
    Crails::Front::Element build_list;
  public:
    Build() : ModelView("Build")
    {
      listen_to(refresh_builds_button.clicked, std::bind(&Build::refresh_builds, this, std::placeholders::_1));
      listen_to(enable_button.clicked,         std::bind(&Build::enable_toggle,  this, std::placeholders::_1));

      page_content.attr("class", "row")
        > (El("div", {{"class","col-lg-12"}})
          > Theme::card("Builds", build_list));

      title_controls.html("").attr("class", "btn-group").inner({
        enable_button,
        edit_link
      });
      enable_button.attr("class", "au-btn au-btn-icon au-btn--blue").visible(false);
      enable_button.css({{"border-top-right-radius","0"},{"border-bottom-right-radius","0"}});
      edit_link.css({{"border-top-left-radius","0"},{"border-bottom-left-radius","0"}});
    }

    void set_enabled_state(bool enabled)
    {
      std::string icon = enabled ? "times"   : "check";
      std::string text = enabled ? "Disable" : "Enable";

      enable_button.html("").inner({
        Theme::fa_icon(icon),
        El("span").text(' ' + text)
      }).visible(true);
      is_enabled = enabled;
    }

    void enable_toggle(client::Event*)
    {
      std::string url = model->get_url();

      if (is_enabled)
        url += "/disable";
      else
        url += "/enable";
      Crails::Front::Ajax::query("POST", url).callbacks({
        [this](const Crails::Front::Ajax&) { set_enabled_state(!is_enabled); },
	[](const Crails::Front::Ajax&) { std::cout << "failed to enable/disable" << std::endl; }
      })();
    }

    void on_model_received()
    {
      refresh_builds();
    }

    void refresh_builds(client::Event* = 0)
    {
      Crails::Front::Ajax::query("GET", model->get_url() + "/builds").callbacks({
        std::bind(&Build::on_builds_fetched,      this, std::placeholders::_1),
        std::bind(&Build::on_builds_fetch_failed, this, std::placeholders::_1)
      })();
    }

    void on_builds_fetched(const Crails::Front::Ajax& ajax)
    {
      auto data = ajax.get_response_as_json();

      if (!data.is_undefined())
      {
        std::cout << "fetched builds" << std::endl;
        std::string                        buildable = data["buildable"];
        std::vector<Crails::Front::Object> builds    = data["builds"];
        Crails::Front::Element tbody("tbody");

	set_enabled_state(buildable == "true");
	for (auto build : builds)
	{
          std::string build_url = build["url"];
          std::string number    = build["number"];

          tbody > El("tr").inner({
            El("td") > El("a", {{"href",build_url},{"target","_blank"}}).text("Build " + number)
          });
	}
	build_list.html("") > tbody;
      }
      else
        std::cout << "fetched builds, but there's no json response" << std::endl;
    }

    void on_builds_fetch_failed(const Crails::Front::Ajax&)
    {
      std::cout << "failed to fetch builds" << std::endl;
    }
  };
}

#endif
