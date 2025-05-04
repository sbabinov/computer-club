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

void events::Event::print(std::ostream& out) const
{
  out << time_ << ' ' << id_;
}

void events::Event::process(club::ComputerClub& club) const
{
  club.logEvent(*this);
}

std::ostream& events::operator<<(std::ostream& out, const Event& event)
{
  event.print(out);
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

events::ClientEvent::ClientEvent(Time time, const std::string& clientName):
  events::Event(time),
  clientName_(clientName)
{}

void events::ClientEvent::print(std::ostream& out) const
{
  events::Event::print(out);
  out << ' ' << clientName_;
}

events::ClientCameEvent::ClientCameEvent(Time time, const std::string& clientName):
  events::ClientEvent(time, clientName)
{
  id_ = 1;
}

void events::ClientCameEvent::process(club::ComputerClub& club) const
{
  events::Event::process(club);
  if (!club.isOpen())
  {
    events::ErrorEvent(club.currentTime(), events::ErrorType::NOT_OPEN_YET).process(club);
  }
  else if (club.hasClient(clientName_))
  {
    events::ErrorEvent(club.currentTime(), events::ErrorType::YOU_SHALL_NOT_PASS).process(club);
  }
  else
  {
    club.addClient(clientName_);
  }
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

void events::ClientSatEvent::print(std::ostream& out) const
{
  events::ClientEvent::print(out);
  out << ' ' << table_;
}

void events::ClientSatEvent::process(club::ComputerClub& club) const
{
  events::Event::process(club);
  if (type_ == Type::INCOMING)
  {
    if (!club.hasClient(clientName_))
    {
      events::ErrorEvent(club.currentTime(), events::ErrorType::CLIENT_UNKNOWN).process(club);
    }
    if (club.isTableOccupied(table_))
    {
      events::ErrorEvent(club.currentTime(), events::ErrorType::PLACE_IS_BUSY).process(club);
    }
    else
    {
      club.assignTable(clientName_, table_);
    }
  }
}

events::ClientWaitingEvent::ClientWaitingEvent(Time time, const std::string& clientName):
  events::ClientEvent(time, clientName)
{
  id_ = 3;
}

void events::ClientWaitingEvent::process(club::ComputerClub& club) const
{
  events::Event::process(club);
  if (club.hasAvailableTable())
  {
    events::ErrorEvent(club.currentTime(), events::ErrorType::I_CAN_WAIT_NO_LONGER).process(club);
  }
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

void events::ClientLeftEvent::process(club::ComputerClub& club) const
{
  events::Event::process(club);
  if (type_ == Type::INCOMING)
  {
    if (!club.hasClient(clientName_))
    {
      events::ErrorEvent(club.currentTime(), events::ErrorType::CLIENT_UNKNOWN).process(club);
    }
    else
    {
      size_t availableTable = club.removeClient(clientName_);
      if (club.isOpen() && (!club.isQueueEmpty()))
      {
        ClientSatEvent(time_, club.getClientFromQueue(), availableTable, ClientSatEvent::Type::OUTCOMING).process(club);
      }
    }
  }
  else
  {
    club.removeClient(clientName_);
  }
}

events::ErrorEvent::ErrorEvent(Time time, ErrorType error):
  events::Event(time),
  error_(error)
{
  id_ = 13;
}

void events::ErrorEvent::print(std::ostream& out) const
{
  events::Event::print(out);
  out << ' ' << convertErrorToString(error_);
}
