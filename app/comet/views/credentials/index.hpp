#ifndef  VIEW_CREDENTIALS_HPP
# define VIEW_CREDENTIALS_HPP

# include "../utility/index_view.hpp"
# include "app/comet/collections/credentials.hpp"

namespace Views
{
  class CredentialIndex : public IndexView<Front::Credentials>
  {
  public:
    CredentialIndex(std::shared_ptr<Front::Credentials> c) : IndexView(c)
    {
    }

    virtual ~CredentialIndex() {}

    std::string              get_title() const          { return "Credentials"; }
    std::string              get_new_model_path() const { return "#/credentials/new"; }
    std::vector<std::string> get_column_labels() const  { return {"name"}; }

    Elements make_columns_for(std::shared_ptr<Front::Credential> model) const
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
