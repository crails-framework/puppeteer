#ifndef  VIEW_MACHINES_HPP
# define VIEW_MACHINES_HPP

# include "../utility/index_view.hpp"
# include "app/comet/collections/machines.hpp"

namespace Views
{
  class MachineIndex : public IndexView<Front::Machines>
  {
  public:
    MachineIndex(std::shared_ptr<Front::Machines> c) : IndexView(c)
    {
    }

    std::string              get_title() const          { return "Machines"; }
    std::string              get_new_model_path() const { return "#/machines/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Front::Machine> model) const
    {
      return {
        Comet::Element("td").inner({
          Comet::Element("a", {{"href",model->get_path()}}).text(model->get_name())
        })
      };
    }
  };
}

#endif
