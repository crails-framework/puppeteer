#ifndef  VIEW_BUILD_HPP
# define VIEW_BUILD_HPP

# include "utility/model_view.hpp"
# include "lib/cheerp-html/views/build.hpp"
# include "../app/builds.hpp"

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

    std::vector<Crails::Front::Object>& get_builds() { return builds; }

    void set_enabled_state(bool value);
    void refresh_builds();

    void on_builds_fetched(const Crails::Front::Ajax& ajax);

    void on_builds_fetch_failed(const Crails::Front::Ajax&)
    {
      std::cout << "failed to fetch builds" << std::endl;
    }

    void on_remove_build_clicked(std::string build_number);

  private:
    std::vector<Crails::Front::Object> builds;
  };
}

#endif
