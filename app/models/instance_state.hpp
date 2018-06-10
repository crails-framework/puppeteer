#ifndef  INSTANCE_STATE_HPP
# define INSTANCE_STATE_HPP

# include <crails/front/archive.hpp>
# include <crails/utils/string.hpp>

class InstanceState
{
public:
  define_serializer(
    archive & needs_restart & needs_configure & processes & states;
  )

  void clear() { processes.clear(); states.clear(); }

  void initialize_from_monit(const std::string& data)
  {
    std::string process_pattern = "Process ";
    size_t pos = data.find(process_pattern);

    while (pos != std::string::npos)
    {
      size_t      end_line_pos = data.find("\n", pos);
      std::string process_name = data.substr(pos + process_pattern.length() + 1,
                                            (end_line_pos - (pos + process_pattern.length()) - 2));
      size_t      start_status = data.find("status", end_line_pos);
      size_t      end_status   = data.find("\n", start_status);
      std::string state        = data.substr(start_status + 6, end_status - (start_status + 6));

      processes.push_back(process_name);
      states.push_back(Crails::strip(state));
      pos = data.find(process_pattern, end_status);
    }
  }

  std::map<std::string, std::string> get_states()
  {
    std::map<std::string, std::string> result;
    auto it_process = processes.begin();
    auto it_state   = states.begin();

    while (it_process != processes.end())
    {
      result[*it_process] = *it_state;
      ++it_process;
      ++it_state;
    }
    return result;
  }

  const std::vector<std::string>& get_processes() const { return processes; }

  bool get_needs_restart()   const { return needs_restart;   }
  bool get_needs_configure() const { return needs_configure; }
  void set_needs_restart(bool value)   { needs_restart   = value; }
  void set_needs_configure(bool value) { needs_configure = value; }

private:
  std::vector<std::string> processes;
  std::vector<std::string> states;
  bool                     needs_restart   = true;
  bool                     needs_configure = true;
};

#endif
