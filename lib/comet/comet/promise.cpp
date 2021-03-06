#include "promise.hpp"
#include "globals.hpp"

using namespace std;
using namespace Comet;

Promise::Promise(std::function<void (std::function<void ()>, std::function<void ()>)> resolver)
{
  client::Object* func = cheerp::Callback([resolver](client::Object* callbacks_ptr)
  {
    Comet::Object callbacks(callbacks_ptr);

    resolver(
      [callbacks]() mutable { callbacks.apply("resolve"); },
      [callbacks]() mutable { callbacks.apply("reject");  }
    );
  });
  new_promise(func);
}

void Promise::new_promise(client::Object* resolver)
{
  prepare_promise_constructor();
  ptr = *(window.apply("_comet_new_promise", resolver));
}

void Promise::prepare_promise_constructor()
{
  static bool is_prepared = false;

  if (!is_prepared)
  {
    client::eval("window._comet_new_promise = function(r) { return new Promise(function(resolve, reject) { return r({ resolve: resolve, reject: reject }); }); }");
    is_prepared = true;
  }
}

Promise Promise::all(const std::vector<Promise>& promises)
{
  Comet::ObjectImpl<client::Array> array;

  for (auto promise : promises)
    array->push(*promise);
  return *(window["Promise"].apply("all", array));
}

Promise Promise::solved_promise()
{
  return Promise([](function<void()> resolve, function<void()>) { resolve(); });
}
