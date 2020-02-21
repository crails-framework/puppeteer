#ifndef  VIEW_BUILDS_HPP
# define VIEW_BUILDS_HPP

# include "utility/index_view.hpp"
# include "../app/builds.hpp"

namespace Views
{
  class Builds : public IndexView<Puppeteer::Builds>
  {
  public:
    std::string              get_title() const          { return "Builds"; }
    std::string              get_new_model_path() const { return "#/builds/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Puppeteer::Build> model) const
    {
      return {
        Crails::Front::Element("td").inner({
          Crails::Front::Element("a", {{"href",model->get_path()}}).text(model->get_name())
        })
      };
    }
  private:
  };
}

#endif
