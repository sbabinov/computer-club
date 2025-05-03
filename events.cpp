#include "events.hpp"
#include "guards.hpp"

bool events::Time::operator<(const Time& other) const
{
  return (this->hours < other.hours) || ((this->hours == other.hours) && (this->minutes < other.minutes));
}

bool events::Time::operator>(const Time& other) const
{
  return other < *this;
}

std::ostream& events::operator<<(std::ostream& out, const Time& time)
{
  guards::ScopeGuard guard(out);
  out << std::setfill('0') << std::setw(2) << time.hours << ':' << std::setw(2) << time.minutes;
  return out;
}
