#include "structs.hpp"
#include <regex>
#include <iomanip>
#include "guards.hpp"

bool structs::Time::operator<(const Time& other) const
{
  return (this->hours < other.hours) || ((this->hours == other.hours) && (this->minutes < other.minutes));
}

bool structs::Time::operator>(const Time& other) const
{
  return other < *this;
}

bool structs::Time::operator>=(const Time& other) const
{
  return !(*this < other);
}

bool structs::Time::operator<=(const Time& other) const
{
  return !(*this > other);
}

structs::Time structs::Time::operator+(const Time& other) const
{
  unsigned short newHours = 0;
  unsigned short newMinutes = 0;
  if (minutes + other.minutes > 59)
  {
    newHours = hours + other.hours + 1;
    newMinutes = minutes + other.minutes - 60;
  }
  else
  {
    newHours = hours + other.hours;
    newMinutes = minutes + other.minutes;
  }
  if (newHours > 23)
  {
    newHours = 0;
  }
  return Time{newHours, newMinutes};
}

structs::Time structs::Time::operator-(const Time& other) const
{
  unsigned short newHours = 0;
  unsigned short newMinutes = 0;
  if (minutes - other.minutes < 0)
  {
    newHours = hours - other.hours - 1;
    newMinutes = minutes - other.minutes + 60;
  }
  else
  {
    newHours = hours - other.hours;
    newMinutes = minutes - other.minutes;
  }
  if (newHours < 0)
  {
    newHours = 23;
  }
  return Time{newHours, newMinutes};
}

std::istream& structs::operator>>(std::istream& in, Time& time)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string strTime = "";
  in >> strTime;
  std::regex regex("^([01][0-9]|2[0-3]):([0-5][0-9])$");
  std::smatch match;
  if (!std::regex_match(strTime, match, regex))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  time.hours = std::stoi(match[1].str());
  time.minutes = std::stoi(match[2].str());
  return in;
}

std::ostream& structs::operator<<(std::ostream& out, const Time& time)
{
  guards::ScopeGuard guard(out);
  out << std::setfill('0') << std::setw(2) << time.hours << ':' << std::setw(2) << time.minutes;
  return out;
}
