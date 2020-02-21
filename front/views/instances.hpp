#ifndef  VIEW_INSTANCES_HPP
# define VIEW_INSTANCES_HPP

# include "utility/index_view.hpp"
# include "../app/instances.hpp"

namespace Views
{
  class Instances : public IndexView<Puppeteer::Instances>
  {
  public:
    std::string              get_title() const          { return "Instances"; }
    std::string              get_new_model_path() const { return "#/instances/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Puppeteer::Instance> model) const
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

