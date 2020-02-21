#ifndef  VIEW_MACHINES_HPP
# define VIEW_MACHINES_HPP

# include "utility/index_view.hpp"
# include "../app/machines.hpp"

namespace Views
{
  class Machines : public IndexView<Puppeteer::Machines>
  {
  public:
    std::string              get_title() const          { return "Machines"; }
    std::string              get_new_model_path() const { return "#/machines/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Puppeteer::Machine> model) const
    {
      return {
        Crails::Front::Element("td").inner({
          Crails::Front::Element("a", {{"href",model->get_path()}}).text(model->get_name())
        })
      };
    }
  };
}

#endif
