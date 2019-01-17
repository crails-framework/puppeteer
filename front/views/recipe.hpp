#ifndef  VIEW_RECIPE_HPP
# define VIEW_RECIPE_HPP

# include "utility/model_view.hpp"
# include "utility/button.hpp"
# include "../app/recipes.hpp"
# include <crails/front/ajax.hpp>

namespace Views
{
  class Recipe : public ModelView<Puppeteer::Recipe>
  {
    Button button_fetch;
    Crails::Front::Element informations, actions;
  public:
    Recipe() : ModelView("Recipe")
    {
      button_fetch.inner({
        Theme::fa_icon("download"),
        El("span").text(" Pull")
      }).add_class("btn-primary");
      actions.add_class("btn-group");
      actions > button_fetch;

      page_content.attr("class", "row").inner({
        El("div", {{"class","col-lg-6"}}).inner({
          Theme::card("Informations", informations)
        }),
	El("div", {{"class","col-lg-6"}}).inner({
          Theme::card("Actions", actions)
	})
      });

      listen_to(button_fetch.clicked, std::bind(&Recipe::on_fetch_clicked, this, std::placeholders::_1));
    }

    void render()
    {
      informations.html("").attr("class", "table table-responsive");
      informations > El("table") > El("tbody").inner({
        El("tr").inner({
          El("td").text("URL"),
          El("td").text(model->get_git_url())
        }),
        El("tr").inner({
          El("td").text("Branch"),
          El("td").text(model->get_git_branch())
        }),
        El("tr").inner({
          El("td").text("Tip"),
          El("td").text(model->get_last_tip())
        })
      });
    }

    void on_model_received()
    {
      render();
    }

    void on_fetch_clicked(client::Event*)
    {
      Crails::Front::Ajax::query("POST", model->get_url() + "/fetch").callbacks({
        std::bind(&Recipe::on_fetched,      this, std::placeholders::_1),
	std::bind(&Recipe::on_fetch_failed, this, std::placeholders::_1)
      })();
    }

    void on_fetched(const Crails::Front::Ajax&)
    {
      std::cout << "recipe fetched" << std::endl;
      model->fetch().then([this]() { render(); });
    }

    void on_fetch_failed(const Crails::Front::Ajax&)
    {
      std::cout << "recipe fetch failed" << std::endl;
    }
  };
}

#endif
