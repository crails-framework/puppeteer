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
