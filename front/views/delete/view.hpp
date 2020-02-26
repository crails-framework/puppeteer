#ifndef  DELETE_VIEW_HPP
# define DELETE_VIEW_HPP

namespace Views
{
  template<class BASE, class MODEL>
  class DeleteView : public BASE
  {
    std::shared_ptr<MODEL> model;
  public:
    virtual void activate(unsigned long id)
    {
      fetch_one<MODEL>(id, [this](std::shared_ptr<MODEL> _model)
      {
        this->model = _model;
	on_model_received();
      });
    }

    virtual void destroy(std::function<void (bool)> callback)
    {
      model->destroy([&](std::shared_ptr<Crails::Front::Http::Response> response)
      {
        if (response->ok())
          on_model_destroyed();
        else
          on_model_destroy_failed(response);
        callback(response->ok());
      });
    }

    virtual void on_model_destroyed()
    {
    }

    virtual void on_model_destroy_failed(std::shared_ptr<Crails::Front::Http::Response>)
    {
    }

    virtual void on_model_received()
    {
    }
  };
}

#endif
