#include "events.hpp"
#include <algorithm>
#include <cctype>

std::string convertErrorToString(events::ErrorType error)
{
  switch (error)
  {
  case events::ErrorType::YOU_SHALL_NOT_PASS:
    return "YouShallNotPass";
  case events::ErrorType::NOT_OPEN_YET:
    return "NotOpenYet";
  case events::ErrorType::CLIENT_UNKNOWN:
    return "ClientUnknown";
  case events::ErrorType::PLACE_IS_BUSY:
    return "PlaceIsBusy";
  case events::ErrorType::I_CAN_WAIT_NO_LONGER:
    return "ICanWaitNoLonger!";
  }
  return "";
}

bool isNameCorrect(const std::string& name)
{
  auto pred = [&](const char ch) -> bool
  {
    return !(std::isalnum(ch) || (ch == '_') || (ch == '-'));
  };
  return (std::find_if(name.cbegin(), name.cend(), pred) == name.cend());
}

events::Event::Event(Time time):
  time_(time),
  id_(0)
{}

events::Time events::Event::getTime() const
{
  return time_;
}

void events::Event::process(club::ComputerClub& club) const
{}

std::ostream& events::operator<<(std::ostream& out, const Event& event)
{
  out << event.time_ << ' ' << event.id_;
  return out;
}

std::istream& events::operator>>(std::istream& in, std::unique_ptr< ClientEvent >& event)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  unsigned short id = 0;
  Time time = {0, 0};
  std::string name = "";
  in >> time >> id >> name;
  if (!in)
  {
    return in;
  }

  if (!isNameCorrect(name))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (id == 1)
  {
    event = std::make_unique< events::ClientCameEvent >(time, name);
  }
  else if (id == 2)
  {
    size_t table;
    in >> table;
    if (in)
    {
      event = std::make_unique< events::ClientSatEvent >(time, name, table, events::ClientSatEvent::Type::INCOMING);
    }
  }
  else if (id == 3)
  {
    event = std::make_unique< events::ClientWaitingEvent >(time, name);
  }
  else if (id == 4)
  {
    event = std::make_unique< events::ClientLeftEvent >(time, name, events::ClientLeftEvent::Type::INCOMING);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& events::operator<<(std::ostream& out, const ClientEvent& event)
{
  out << static_cast< const events::Event& >(event) << ' ' << event.clientName_;
  return out;
}

events::ClientEvent::ClientEvent(Time time, const std::string& clientName):
  events::Event(time),
  clientName_(clientName)
{}

events::ClientCameEvent::ClientCameEvent(Time time, const std::string& clientName):
  events::ClientEvent(time, clientName)
{
  id_ = 1;
}

events::ClientSatEvent::ClientSatEvent(Time time, const std::string& clientName, size_t table, Type type):
  events::ClientEvent(time, clientName),
  table_(table),
  type_(type)
{
  if (type_ == Type::INCOMING)
  {
    id_ = 2;
  }
  else
  {
    id_ = 12;
  }
}

std::ostream& events::operator<<(std::ostream& out, const ClientSatEvent& event)
{
  out << static_cast< const events::ClientEvent& >(event) << ' ' << event.table_;
  return out;
}

events::ClientWaitingEvent::ClientWaitingEvent(Time time, const std::string& clientName):
  events::ClientEvent(time, clientName)
{
  id_ = 3;
}

events::ClientLeftEvent::ClientLeftEvent(Time time, const std::string& clientName, Type type):
  events::ClientEvent(time, clientName),
  type_(type)
{
  if (type_ == Type::INCOMING)
  {
    id_ = 4;
  }
  else
  {
    id_ = 11;
  }
}

events::ErrorEvent::ErrorEvent(Time time, ErrorType error):
  events::Event(time),
  error_(error)
{
  id_ = 13;
}

std::ostream& events::operator<<(std::ostream& out, const ErrorEvent& event)
{
  out << static_cast< const events::Event& >(event) << ' ' << convertErrorToString(event.error_);
  return out;
}
