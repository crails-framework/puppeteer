#ifndef  INDEX_VIEW_HPP
# define INDEX_VIEW_HPP

# include "lib/cheerp-html/views/utility/index_view.hpp"
# include "front/resources/elements/breadcrumbs.hpp"
# include <iostream>

namespace Views
{
  template<typename COLLECTION>
  class IndexView : public HtmlTemplate::IndexView
  {
  public:
    typedef typename COLLECTION::Model          Model;
    typedef std::vector<Crails::Front::Element> Elements;

    virtual Elements make_columns_for(std::shared_ptr<Model>) const = 0;

    void activate()
    {
      collection.fetch().then([this]() { on_fetched(); });
      initialize_breadcrumbs();
    }

    void attached() // TODO delete this method
    {
      activate();
    }

    virtual void on_fetched()
    {
      std::cout << "IndexView::on_fetched" << std::endl;
      render_rows();
    }

    void render_rows()
    {
      tbody.empty();
      collection.each([this](std::shared_ptr<Model> model)
      {
        Crails::Front::Element row("tr");

        row.inner(make_columns_for(model));
        tbody > row;
      });
    }

    virtual void initialize_breadcrumbs()
    {
      Breadcrumbs::set_index_crumbs(
        COLLECTION::Model::get_scope(),
        COLLECTION::Model::get_index_path()
      );
    }

  private:
    COLLECTION collection;
  };
}

#endif
