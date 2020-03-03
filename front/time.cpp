#include "time.hpp"
#include <sstream>
#include <iomanip>

using namespace std;

string get_date_string(time_t timestamp, const string& format)
{
  stringstream stream;
  tm* tm = localtime(&timestamp);

  stream << put_time(tm, format.c_str());
  return stream.str();
}

string get_duration_string(time_t duration)
{
  stringstream stream;
  unsigned int seconds = duration;
  unsigned int minutes = seconds / 60;
  unsigned int hours   = minutes / 60;

  if (hours > 0)
    stream << hours << 'h';
  if (minutes > 0)
    stream << setw(2) << setfill('0') << (minutes % 60) << 'm';
  stream << setw(2) << setfill('0') << (seconds % 60);
  if (!minutes)
    stream << 's';
  return stream.str();
}
