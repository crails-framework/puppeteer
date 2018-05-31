#ifndef  VIEW_MACHINES_HPP
# define VIEW_MACHINES_HPP

# include "utility/table_view.hpp"
# include "utility/theme.hpp"
# include <iostream>
# include "../app/machines.hpp"

namespace Views
{
  class Machines : public TableView<Puppeteer::Machines>
  {
  public:
    Machines()
    {
      title = "Machines";
    }

    void attached()
    {
      std::cout << "machine view attached maggle" << std::endl;
      auto list = el.find("tbody");
      std::cout << "number of tbody found: " << list.size() << std::endl;
      TableView::attached();
    }

    std::string get_new_model_path() const { return "#/machines/new"; }

    std::vector<std::string> get_columns() const { return {"name"}; }

    std::vector<El> make_columns_for(std::shared_ptr<Puppeteer::Machine> model) const
    {
      return {
        El("td").inner({
          El("a", {{"href",model->get_path()}}).text(model->get_name())
        })
      };
    }
  };
}

#endif
