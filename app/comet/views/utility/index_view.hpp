#ifndef  INDEX_VIEW_HPP
# define INDEX_VIEW_HPP

# include "html/views/utility/index_view.hpp"
# include "app/comet/views/resources/elements/breadcrumbs.hpp"
# include <iostream>

namespace Views
{
  template<typename COLLECTION>
  class IndexView : public HtmlTemplate::IndexView, public Comet::Listener
  {
  public:
    typedef typename COLLECTION::Model  Model;
    typedef std::vector<Comet::Element> Elements;

    virtual Elements make_columns_for(std::shared_ptr<Model>) const = 0;

    IndexView(std::shared_ptr<COLLECTION> c) : collection(c)
    {
      listen_to(c->synced, [this]() { on_fetched(); });
    }

    void activate()
    {
      initialize_breadcrumbs();
    }

    virtual void on_fetched()
    {
      std::cout << "IndexView::on_fetched" << std::endl;
      render_rows();
    }

    void render_rows()
    {
      tbody.empty();
      collection->each([this](std::shared_ptr<Model> model)
      {
        Comet::Element row("tr");

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
    std::shared_ptr<COLLECTION> collection;
  };
}

#endif
