#ifndef  TABLE_VIEW_HPP
# define TABLE_VIEW_HPP

# include "template_view.hpp"
# include "theme.hpp"
# include <crails/front/mvc/collection.hpp>

namespace Views
{
  template<typename COLLECTION>
  class TableView : public TemplateView
  {
  public:
    typedef typename COLLECTION::Model Model;

    virtual std::string              get_new_model_path() const = 0;
    virtual std::vector<std::string> get_columns() const = 0;
    virtual std::vector<El>          make_columns_for(std::shared_ptr<Model>) const = 0;

    TableView() : TemplateView("tpl1")
    {
      typedef Crails::Front::Element El;

      thead = El("thead");
      tbody = El("tbody");

      el.attr("class", "container-fluid");
    }

    void initialize_table()
    {
      el.inner({
        Theme::title1(title, El("a", {{"href",get_new_model_path()},{"class","au-btn au-btn-icon au-btn--blue"}}).inner({
          El("i", {{"class","zmdi zmdi-plus"}}),
          El("span").text("Add")
        })),
        El("div", {{"class","row"}}).inner({
          El("div", {{"class","col-lg-12"}}).inner({
            Theme::responsive_table(thead, tbody)
          })
        })
      });
    }

    void initialize_thead()
    {
      auto column_labels = get_columns();
      std::vector<Crails::Front::Element> columns;

      columns.reserve(column_labels.size());
      for (auto label : column_labels)
        columns.push_back(Crails::Front::Element("th").text(label));
      thead.inner({El("tr").inner(columns)});
    }

    void render_tbody()
    {
      typedef Crails::Front::Element El;
      std::vector<Crails::Front::Element> rows;

      rows.reserve(collection.count());
      collection.each([this, &rows](std::shared_ptr<Model> model)
      {
        Crails::Front::Element row("tr");

        row.inner(make_columns_for(model));
        rows.push_back(row);
      });
      tbody.html("").inner(rows);
    }

    void attached()
    {
      initialize_table();
      initialize_thead();
      collection.fetch({
        std::bind(&TableView<COLLECTION>::on_fetched,     this, std::placeholders::_1),
        std::bind(&TableView<COLLECTION>::on_fetch_error, this, std::placeholders::_1)
      });
    }

    void on_fetched(const Crails::Front::Ajax&)
    {
      std::cout << "machine collection fetched " << collection.count() << std::endl;
      render_tbody();
    }

    void on_fetch_error(const Crails::Front::Ajax&)
    {
      std::cout << "machine collection fetch failed" << std::endl;
    }

  protected:
    std::string title;
    COLLECTION collection;
    Crails::Front::Element thead, tbody;
  };
}

#endif
