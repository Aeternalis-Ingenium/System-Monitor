#include <string>

#include "format.h"

std::string Format::ElapsedTime(long int seconds) {
  int seconds_in_1min{60}, seconds_in_1hour{3600};
  long hours, minutes;

  hours = seconds / seconds_in_1hour;
  seconds = seconds % seconds_in_1hour;
  minutes = seconds / seconds_in_1min;
  seconds = seconds % seconds_in_1min;

  // Reformat the time to be HH:MM:SS
  std::string ss = std::to_string(seconds);
  ss.insert(0, 2 - ss.length(), '0');

  std::string mm = std::to_string(minutes);
  mm.insert(0, 2 - mm.length(), '0');

  std::string hh = std::to_string(hours);
  hh.insert(0, 2 - hh.length(), '0');

  return hh + ":" + mm + ":" + ss + "\n";
}