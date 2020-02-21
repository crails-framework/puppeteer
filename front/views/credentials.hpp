#ifndef  VIEW_CREDENTIALS_HPP
# define VIEW_CREDENTIALS_HPP

# include "utility/index_view.hpp"
# include "../app/credentials.hpp"

namespace Views
{
  class Credentials : public IndexView<Puppeteer::Credentials>
  {
  public:
    std::string              get_title() const          { return "Credentials"; }
    std::string              get_new_model_path() const { return "#/credentials/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Puppeteer::Credential> model) const
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
