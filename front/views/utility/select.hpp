#ifndef  SELECT_ELEMENT_HPP
# define SELECT_ELEMENT_HPP

# include <crails/front/element.hpp>
# include <crails/front/mvc/collection.hpp>

namespace Views
{
  template<typename COLLECTION>
  class CollectionSelect : public Crails::Front::Element
  {
  public:
    typedef typename COLLECTION::Model Model;

    CollectionSelect() : Element("select")
    {
    }

    virtual std::string label_for(const Model& model) = 0;

    void render()
    {
      collection.fetch().then([this]() { on_fetched(); });
    }

  protected:
    void on_fetched()
    {
      std::vector<Crails::Front::Element> options;

      collection.each([this, &options](std::shared_ptr<Model> model)
      {
        Crails::Front::Element option("option");

        option.attr("value", boost::lexical_cast<std::string>(model->get_id()));
        option.text(label_for(*model));
        options.push_back(option);
      });
      html("").inner(options);
    }

  private:
    COLLECTION collection;
  };

  template<typename COLLECTION>
  class CollectionSelectWithName : public CollectionSelect<COLLECTION>
  {
    std::string label_for(const typename CollectionSelect<COLLECTION>::Model& model)
    {
      return model.get_name();
    }
  };
}

#endif
