#ifndef  VIEW_CREDENTIALS_HPP
# define VIEW_CREDENTIALS_HPP

# include "utility/table_view.hpp"
# include "utility/theme.hpp"
# include <iostream>
# include "../app/credentials.hpp"

namespace Views
{
  class Credentials : public TableView<Puppeteer::Credentials>
  {
  public:
    Credentials()
    {
      title = "Credentials";
    }

    std::string get_new_model_path() const { return "#/credentials/new"; }

    std::vector<std::string> get_columns() const { return {"name"}; }

    std::vector<El> make_columns_for(std::shared_ptr<Puppeteer::Credential> model) const
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
