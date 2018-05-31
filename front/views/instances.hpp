#ifndef  VIEW_INSTANCES_HPP
# define VIEW_INSTANCES_HPP

# include "utility/table_view.hpp"
# include "../app/instances.hpp"

namespace Views
{
  class Instances : public TableView<Puppeteer::Instances>
  {
  public:
    Instances()
    {
      title = "Instances";
    }

    std::string get_new_model_path() const { return "#/instances/new"; }

    std::vector<std::string> get_columns() const { return {"name"}; }

    std::vector<El> make_columns_for(std::shared_ptr<Puppeteer::Instance> model) const
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

