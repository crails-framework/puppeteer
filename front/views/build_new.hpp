#ifndef  VIEW_BUILD_NEW_HPP
# define VIEW_BUILD_NEW_HPP

# include "utility/model_form.hpp"
# include "utility/select.hpp"
# include "../app/builds.hpp"
# include "../app/recipes.hpp"

namespace Views
{
  class BuildNew : public ModelForm<Puppeteer::Build>
  {
    Crails::Front::Element input_name, input_git, input_branch, input_options;
    CollectionSelectWithName<Puppeteer::Recipes> input_recipe;
  public:
    BuildNew() : ModelForm("New build")
    {
      input_name    = El("input", {{"name","build_name"},{"type","text"},{"class","form-control"}});
      input_git     = El("input", {{"name","build_git"},{"type","text"},{"class","form-control"}});
      input_branch  = El("input", {{"name","build_branch"},{"type","text"},{"class","form-control"}});
      input_options = El("textarea", {{"name","build_options"},{"class","form-control"}});
      input_recipe.add_class("form-control");
    }

    std::map<std::string, El> get_inputs()
    {
      return {
        {"name",          input_name},
        {"recipe",        input_recipe},
        {"git url",       input_git},
        {"git branch",    input_branch},
        {"build options", input_options}
      };
    }

    void update_model_attributes()
    {
      model->set_name(input_name.get_value());
      model->set_git(input_git.get_value());
      model->set_branch(input_branch.get_value());
      model->set_options(input_options.get_value());
      model->set_recipe_id(boost::lexical_cast<unsigned long>(input_recipe.get_value()));
    }

    void update_form_attributes()
    {
      input_name.value(model->get_name());
      input_git.value(model->get_git());
      input_branch.value(model->get_branch());
      input_options.value(model->get_options());
      input_recipe.value(boost::lexical_cast<std::string>(model->get_recipe_id()));
    }

    void attached()
    {
      ModelForm::attached();
      input_recipe.render();
    }
  };
}

#endif
