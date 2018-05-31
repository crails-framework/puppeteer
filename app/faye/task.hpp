#ifndef  SYNC_TASK_HPP
# define SYNC_TASK_HPP

# include <string>
# include <crails/datatree.hpp>

namespace Sync
{
  class Task
  {
    const unsigned int notification_step_perc = 5;

    std::string  id;
    unsigned int last_notification = 0;
    unsigned int task_progress     = 0;
    unsigned int task_count        = 1;
  public:
    DataTree     metadata;

    Task(unsigned int task_count);
    ~Task();

    unsigned int notification_step() const;
    void increment(unsigned int progress = 1);
    void notify();
  };
}

#endif
