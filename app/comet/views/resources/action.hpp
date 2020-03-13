#ifndef  STRUCT_ACTION_HPP
# define STRUCT_ACTION_HPP

# include <string>

struct Action
{
  Action() {}
  Action(const std::string& _icon, const std::string& _label, std::function<void ()> _callback) :
    icon(_icon), label(_label), callback(_callback) {}

  std::string            icon;
  std::string            label;
  std::function<void ()> callback;
};

typedef std::list<Action> Actions;

#endif
