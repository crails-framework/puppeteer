#ifndef  VIEW_BUILDS_HPP
# define VIEW_BUILDS_HPP

# include "../utility/index_view.hpp"
# include "app/comet/collections/builds.hpp"

namespace Views
{
  class BuildIndex : public IndexView<Front::Builds>
  {
  public:
    BuildIndex(std::shared_ptr<Front::Builds> c) : IndexView(c)
    {
    }

    virtual ~BuildIndex() {}

    std::string              get_title() const          { return "Builds"; }
    std::string              get_new_model_path() const { return "#/builds/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Front::Build> model) const
    {
      return {
        Comet::Element("td").inner({
          Comet::Element("a", {{"href",model->get_path()}}).text(model->get_name())
        })
      };
    }
  private:
  };
}

#endif
