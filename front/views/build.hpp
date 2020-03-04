#ifndef  VIEW_BUILD_HPP
# define VIEW_BUILD_HPP

# include "utility/model_view.hpp"
# include "lib/cheerp-html/views/build.hpp"
# include "../app/builds.hpp"

namespace Views
{
  class Build : public ModelView<Puppeteer::Build, HtmlTemplate::Build>
  {
    std::string job_url;
  public:
    Build()
    {
    }

    void initialize_breadcrumbs();

    void on_model_received()
    {
      refresh_builds();
    }

    const std::string get_title() const
    {
      if (model)
        return model->get_name();
      return HtmlTemplate::Build::get_title();
    }

    std::vector<Crails::Front::Object>& get_builds() { return builds; }
    std::string get_badge_for_build(std::string);

    void set_enabled_state(bool value);
    void refresh_builds();

    void on_builds_fetched(const Crails::Front::Ajax& ajax);

    void on_builds_fetch_failed(const Crails::Front::Ajax&)
    {
      std::cout << "failed to fetch builds" << std::endl;
    }

    void on_enable_build_clicked(bool value);
    void on_remove_build_clicked(std::string build_number);

  private:
    std::vector<Crails::Front::Object> builds;
  };
}

#endif
