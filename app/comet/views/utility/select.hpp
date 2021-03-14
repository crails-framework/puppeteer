#ifndef  SELECT_ELEMENT_HPP
# define SELECT_ELEMENT_HPP

# include <comet/custom_element.hpp>
# include <comet/mvc/collection.hpp>
# include <comet/lexical_cast.hpp>

namespace Views
{
  template<typename COLLECTION>
  class CollectionSelect : public Comet::CustomElement
  {
    bool _with_empty_option = false;
  public:
    typedef typename COLLECTION::Model ModelType;

    CollectionSelect() : CustomElement("select")
    {
    }

    virtual ~CollectionSelect() {}

    virtual std::string label_for(const ModelType& model) = 0;

    void bind_attributes()
    {
      CustomElement::bind_attributes();
      fetch();
    }

    void trigger_binding_updates()
    {
      CustomElement::trigger_binding_updates();
      on_fetched();
    }

    void fetch()
    {
      collection.fetch().then([this]() { on_fetched(); });
    }

    void with_empty_option(bool val)
    {
      if (_with_empty_option != val)
      {
        _with_empty_option = val;
        on_fetched();
      }
    }

    template<typename PARAM>
    void set_value(PARAM __v)
    {
      typedef Comet::ObjectImpl<client::HTMLOptionElement> OptionEl;
      auto str_value      = Comet::lexical_cast<std::string>(__v);
      auto value_elements = find("option[value='" + str_value + "']");

      if (value_elements.size() > 0)
      {
        client::HTMLElement* html_element = *(value_elements[0]);
        OptionEl             option(html_element);

	option->set_selected(true);
      }
    }

    std::string get_value()
    {
      auto value_elements = find("option:checked");

      if (value_elements.size() > 0)
        return value_elements[0].get_value();
      return "";
    }

  protected:
    void on_fetched()
    {
      std::vector<Comet::Element> options;

      if (_with_empty_option)
      {
        Comet::Element option("option");

        option.attr("value", "0");
        option.text("");
        options.push_back(option);
      }
      collection.each([this, &options](std::shared_ptr<ModelType> model)
      {
        Comet::Element option("option");

        option.attr("value", Comet::lexical_cast<std::string>(model->get_id()));
        option.text(label_for(*model));
        options.push_back(option);
      });
      empty().inner(options);
    }

  private:
    COLLECTION collection;
  };

  template<typename COLLECTION>
  class CollectionSelectWithName : public CollectionSelect<COLLECTION>
  {
    std::string label_for(const typename COLLECTION::Model& model)
    {
      return model.get_name();
    }
  };
}

#endif
