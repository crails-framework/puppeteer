#ifndef  VIEW_RECIPE_NEW_HPP
# define VIEW_RECIPE_NEW_HPP

# include <boost/lexical_cast.hpp>
# include "utility/model_form.hpp"
# include "utility/select.hpp"
# include "../app/recipes.hpp"
# include "../app/credentials.hpp"

namespace Views
{
  class RecipeNew : public ModelForm<Puppeteer::Recipe>
  {
    Crails::Front::Element input_name, input_git_url, input_git_branch;
    CollectionSelectWithName<Puppeteer::Credentials> input_credential;
  public:
    RecipeNew() : ModelForm("New recipe")
    {
      input_name       = El("input", {{"name","recipe_name"},{"type","text"},{"class","form-control"}});
      input_git_url    = El("input", {
        {"name","recipe_git_url"},{"type","text"},{"class","form-control"},{"placeholder","https://"}
      });
      input_git_branch = El("input", {
        {"name","recipe_git_branch"},{"type","text"},{"class","form-control"},{"placeholder","master"}
      });
      input_credential.add_class("form-control");
      input_credential.with_empty_option(true);
    }

    std::unordered_map<std::string, El> get_inputs()
    {
      return {
        {"name",        input_name},
        {"git url",     input_git_url },
        {"git branch",  input_git_branch },
        {"credentials", input_credential }
      };
    }

    void update_model_attributes()
    {
      model->set_name(input_name.get_value());
      model->set_git_url(input_git_url.get_value());
      model->set_git_branch(input_git_branch.get_value());
      model->set_credential_id( boost::lexical_cast<unsigned long>(input_credential.get_value()) );
    }

    void update_form_attributes()
    {
      input_name.value(model->get_name());
      input_git_url.value(model->get_git_url());
      input_git_branch.value(model->get_git_branch());
      input_credential.value( boost::lexical_cast<std::string>(model->get_credential_id()) );
    }

    void attached()
    {
      ModelForm::attached();
      input_credential.render();
    }
  private:
  };
}

#endif
