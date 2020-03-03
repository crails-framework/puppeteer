#ifndef  FRONT_MY_TIME_HPP
# define FRONT_MY_TIME_HPP

# include <string>
# include <ctime>

std::string get_date_string(std::time_t, const std::string& format = "%c %Z");

#endif
