#ifndef  VIEW_INSTANCES_HPP
# define VIEW_INSTANCES_HPP

# include "../utility/index_view.hpp"
# include "app/comet/collections/instances.hpp"

namespace Views
{
  class InstanceIndex : public IndexView<Front::Instances>
  {
  public:
    InstanceIndex(std::shared_ptr<Front::Instances> c) : IndexView(c)
    {
    }

    InstanceIndex() {}

    virtual ~InstanceIndex() {}

    std::string              get_title() const          { return "Instances"; }
    std::string              get_new_model_path() const { return "#/instances/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Front::Instance> model) const
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

