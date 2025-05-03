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

events::Event::Event(Time time):
  time_(time)
{}

events::ClientEvent::ClientEvent(Time time, const std::string& clientName):
  events::Event(time),
  clientName_(clientName)
{}

events::ClientCameEvent::ClientCameEvent(Time time, const std::string& clientName):
  events::ClientEvent(time, clientName)
{}

events::ClientSatEvent::ClientSatEvent(Time time, const std::string& clientName, Type type):
  events::ClientEvent(time, clientName),
  type_(type)
{}

events::ClientWaitingEvent::ClientWaitingEvent(Time time, const std::string& clientName):
  events::ClientEvent(time, clientName)
{}

events::ClientLeftEvent::ClientLeftEvent(Time time, const std::string& clientName, Type type):
  events::ClientEvent(time, clientName),
  type_(type)
{}

events::ErrorEvent::ErrorEvent(Time time, ErrorType error):
  events::Event(time),
  error_(error)
{}
