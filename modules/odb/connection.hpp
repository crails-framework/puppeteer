#ifndef  ODB_CONNECTION_HPP
# define ODB_CONNECTION_HPP

# include "config/odb.hpp"
# include <crails/safe_ptr.hpp>
# include <odb/pgsql/query.hxx>
# include <odb/result.hxx>
# include <odb/database.hxx>
# include "transaction.hpp"
# include "exception.hpp"
# include "model.hpp"
# include <crails/utils/timer.hpp>
# ifdef USE_FAYE_SYNC
#   include "app/faye/transaction.hpp"
# endif

namespace Db
{
  class Connection
  {
  public:
    static thread_local safe_ptr<Db::Connection> instance;

    Connection();
    ~Connection();

    float time = 0.f;

    template<typename MODEL>
    void start_transaction_for()
    {
      transaction.require(MODEL().get_database_name());
    }

    template<typename MODEL>
    void start_transaction_for(const MODEL& model)
    {
      transaction.require(model.get_database_name());
    }

    void commit();
    void rollback();

    template<typename MODEL>
    unsigned long count(podb::query<MODEL> query = podb::query<MODEL>(true))
    {
      start_transaction_for<MODEL>();
      return transaction.get_database()
        .query_value<typename MODEL::Count>(query).value;
    }

    template<typename MODEL_PTR>
    bool find_one(MODEL_PTR& model, podb::query<typename MODEL_PTR::element_type> query = podb::query<typename MODEL_PTR::element_type>(true))
    {
      typedef typename MODEL_PTR::element_type MODEL;
      Utils::Timer timer;

      start_transaction_for<MODEL>();
      model = transaction.get_database().query_one<MODEL>(query);
      time += timer.GetElapsedSeconds();
      return model.get() != 0;
    }

    template<typename MODEL_PTR>
    bool find_one(MODEL_PTR& model, Db::id_type id)
    {
      return find_one(model, podb::query<typename MODEL_PTR::element_type>::id == id);
    }

    template<typename MODEL>
    bool find(odb::result<MODEL>& results, podb::query<MODEL> query = podb::query<MODEL>(true))
    {
      Utils::Timer timer;

      start_transaction_for<MODEL>();
      results = transaction.get_database().query<MODEL>(query);
      time += timer.GetElapsedSeconds();
      return results.size() > 0;
    }

    template<typename MODEL>
    void save(MODEL& model)
    {
      start_transaction_for(model);
      model.save(transaction.get_database());
#ifdef USE_FAYE_SYNC
      sync_transaction.save(model);
#endif
    }

    template<typename MODEL>
    void destroy(MODEL& model)
    {
      Utils::Timer timer;

      model.on_dependent_destroy(model.get_id());
      start_transaction_for(model);
      try
      {
        model.destroy(transaction.get_database());
#ifdef USE_FAYE_SYNC
	sync_transaction.destroy(model);
#endif
      }
      catch (const odb::object_not_persistent& e)
      {
        Odb::throw_exception(model, e.what());
      }
      time += timer.GetElapsedSeconds();
    }

    template<typename MODEL>
    void destroy(podb::query<MODEL> query = podb::query<MODEL>(true))
    {
      odb::result<MODEL> results;

      if (find(results, query))
      {
        for (auto& model : results)
          destroy(model);
      }
    }

    Transaction       transaction;
#ifdef USE_FAYE_SYNC
    Sync::Transaction sync_transaction;
#endif
  };
}

#endif
