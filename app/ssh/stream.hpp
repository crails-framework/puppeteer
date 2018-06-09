#ifndef  SYNC_STREAM_HPP
# define SYNC_STREAM_HPP

# include <sstream>
# include <crails/sync/task.hpp>
# include <iostream>

namespace Sync
{
  class Stream
  {
    Sync::Task*       task   = nullptr;
    std::ostream*     output = nullptr;
    std::stringstream stream;
  public:
    Stream(Sync::Task& task) : task(&task)
    {}

    Stream(std::ostream& output) : output(&output)
    {}

    Stream()
    {
      stream.setstate(std::ios_base::badbit);
    }

    template<typename T>
    Stream& operator<<(const T& v)
    {
      std::cout << v;
      stream << v;
      if (stream.str().find('\n') != std::string::npos)
        on_new_line();
      return *this;
    }

  private:
    void on_new_line()
    {
      if (task)
        task->notify(stream.str());
      else if (output)
        *output << stream.str();
      stream.str(std::string());
    }
  };
}

#endif
