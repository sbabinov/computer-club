#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <ostream>
#include <iomanip>

namespace events
{
  struct Time
  {
    unsigned short hours;
    unsigned short minutes;

    bool operator<(const Time& other) const;

    bool operator>(const Time& other) const;
  };
  std::ostream& operator<<(std::ostream& out, const Time& time);
}

#endif
