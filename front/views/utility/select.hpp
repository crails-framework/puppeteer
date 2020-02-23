#ifndef  SELECT_ELEMENT_HPP
# define SELECT_ELEMENT_HPP

# include <crails/front/custom_element.hpp>
# include <crails/front/mvc/collection.hpp>

namespace Views
{
  template<typename COLLECTION>
  class CollectionSelect : public Crails::Front::CustomElement
  {
    bool _with_empty_option = false;
  public:
    typedef typename COLLECTION::Model Model;

    CollectionSelect() : CustomElement("select")
    {
    }

    virtual std::string label_for(const Model& model) = 0;

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
      typedef Crails::Front::ObjectImpl<client::HTMLOptionElement> OptionEl;
      auto str_value      = boost::lexical_cast<std::string>(__v);
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
      std::vector<Crails::Front::Element> options;

      if (_with_empty_option)
      {
        Crails::Front::Element option("option");

        option.attr("value", "0");
        option.text("");
        options.push_back(option);
      }
      collection.each([this, &options](std::shared_ptr<Model> model)
      {
        Crails::Front::Element option("option");

        option.attr("value", boost::lexical_cast<std::string>(model->get_id()));
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
    std::string label_for(const typename CollectionSelect<COLLECTION>::Model& model)
    {
      return model.get_name();
    }
  };
}

#endif
