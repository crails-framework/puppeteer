#ifndef  FRONT_THEME_HPP
# define FRONT_THEME_HPP

# include <crails/front/element.hpp>

namespace Views
{
  struct Theme
  {
    typedef Crails::Front::Element El;

    static Crails::Front::Element title1(const std::string& title, Crails::Front::Element controls)
    {
      return title1(El("h2", {{"class","title-1"}}).text(title), controls);
    }

    static Crails::Front::Element title1(Crails::Front::Element title, Crails::Front::Element controls)
    {
      return El("div", {{"class","row"}}).inner({
        El("div", {{"class","col-lg-12"}}).inner({
          El("div", {{"class","overview-wrap"},{"style","padding-bottom:20px"}}).inner({
            title,
            controls
          })
        })
      });
    }

    static Crails::Front::Element responsive_table(Crails::Front::Element& thead, Crails::Front::Element& tbody)
    {
      return El("div", {{"class","table-responsive table--no-card m-b-40"}}).inner({
        El("table", {{"class","table table-borderless table-striped table-earning"}}).inner({
          thead,
	  tbody
	})
      });
    }

    static Crails::Front::Element card(const std::string& title, Crails::Front::Element& content)
    {
      return El("div", {{"class", "card"}}).inner({
        El("div", {{"class", "card-header"}}).inner({
          El("strong").text(title)
        }),
        El("div", {{"class", "card-body card-block"}}).inner({
          content
        })
      });
    }

    static Crails::Front::Element form_group(const std::string& label, Crails::Front::Element& input)
    {
      std::string input_name = input.attr("name");

      return El("div", {{"class","form-group"}}).inner({
        El("label", {{"for",input_name}, {"class","form-control-label"}}).text(label),
        input
      });
    }

    static Crails::Front::Element fa_icon(const std::string& name, const std::string& suffix = "")
    {
      std::string klass = "fa fa-" + name + ' ' + suffix;

      return El("i", {{"class",klass}});
    }
  };
}

#endif
