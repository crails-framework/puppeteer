#ifndef  VIEW_BUILDS_HPP
# define VIEW_BUILDS_HPP

# include "utility/table_view.hpp"
# include "../app/builds.hpp"

namespace Views
{
  class Builds : public TableView<Puppeteer::Builds>
  {
  public:
    Builds()
    {
      title = "Builds";
    }

    std::string get_new_model_path() const { return "#/builds/new"; }

    std::vector<std::string> get_columns() const { return {"name"}; }

    std::vector<El> make_columns_for(std::shared_ptr<Puppeteer::Build> model) const
    {
      return {
        El("td").inner({
          El("a", {{"href",model->get_path()}}).text(model->get_name())
        })
      };
    }
  private:
  };
}

#endif
