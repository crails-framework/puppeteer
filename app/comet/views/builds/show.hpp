#ifndef  VIEW_BUILD_HPP
# define VIEW_BUILD_HPP

# include "../utility/model_view.hpp"
# include "html/views/builds/show.hpp"
# include "app/comet/models/build.hpp"

namespace Views
{
  class BuildShow : public ModelView<Front::Build, HtmlTemplate::BuildShow>
  {
    std::string job_url;
  public:
    BuildShow()
    {
    }

    virtual ~BuildShow() {}

    void initialize_breadcrumbs();

    void on_model_received()
    {
      refresh_builds();
    }

    const std::string get_title() const
    {
      if (model)
        return model->get_name();
      return HtmlTemplate::BuildShow::get_title();
    }

    std::vector<Comet::Object>& get_builds() { return builds; }
    std::string get_badge_for_build(std::string);

    void set_enabled_state(bool value);
    void refresh_builds();

    void on_builds_fetched(const Comet::Http::Response&);

    void on_builds_fetch_failed(const Comet::Http::Response&)
    {
      std::cout << "failed to fetch builds" << std::endl;
    }

    void on_enable_build_clicked(bool value);
    void on_remove_build_clicked(std::string build_number);

  private:
    std::vector<Comet::Object> builds;
  };
}

#endif
