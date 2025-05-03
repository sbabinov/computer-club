#include "events.hpp"
#include "guards.hpp"
#include "delimiters.hpp"

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

bool events::Time::operator<(const Time& other) const
{
  return (this->hours < other.hours) || ((this->hours == other.hours) && (this->minutes < other.minutes));
}

bool events::Time::operator>(const Time& other) const
{
  return other < *this;
}

bool events::Time::operator>=(const Time& other) const
{
  return !(*this < other);
}

bool events::Time::operator<=(const Time& other) const
{
  return !(*this > other);
}

std::istream& events::operator>>(std::istream& in, Time& time)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using del = delimiters::CharDelimiter;
  Time temp = {0, 0};
  in >> temp.hours >> del{':'} >> temp.minutes;
  if (in)
  {
    time = std::move(temp);
  }
  return in;
}

std::ostream& events::operator<<(std::ostream& out, const Time& time)
{
  guards::ScopeGuard guard(out);
  out << std::setfill('0') << std::setw(2) << time.hours << ':' << std::setw(2) << time.minutes;
  return out;
}

events::Event::Event(Time time):
  time_(time),
  id_(0)
{}

events::Time events::Event::getTime() const
{
  return time_;
}

std::ostream& events::operator<<(std::ostream& out, const Event& event)
{
  out << event.time_ << ' ' << event.id_;
  return out;
}

std::istream& events::operator>>(std::istream& in, Event& event)
{

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

events::ClientSatEvent::ClientSatEvent(Time time, const std::string& clientName, Type type):
  events::ClientEvent(time, clientName),
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

events::ComputerClub::ComputerClub(size_t nTables, std::pair< Time, Time > workingTime, size_t price):
  nTables_(nTables),
  price_(price),
  workingTime_(workingTime),
  currentTime_(),
  tables_(),
  clients_(),
  waitingClients_()
{}

bool events::ComputerClub::isOpen() const
{
  return (workingTime_.first <= currentTime_) && (currentTime_ <= workingTime_.second);
}

bool events::ComputerClub::hasClient(const std::string& name) const
{
  return clients_.find(name) != clients_.cend();
}

bool events::ComputerClub::hasAvailableTable() const
{
  return nTables_ < clients_.size();
}

bool events::ComputerClub::isTableOccupied(size_t table) const
{
  return tables_.find(table) != tables_.cend();
}

bool events::ComputerClub::isQueueEmpty() const
{
  return waitingClients_.empty();
}

void events::ComputerClub::addClient(const std::string& name)
{
  clients_.insert(name);
}

void events::ComputerClub::assignTable(const std::string& name, size_t table)
{
  tables_[table] = name;
}

size_t events::ComputerClub::removeClient(const std::string& name)
{
  size_t table = 0;
  auto it = tables_.begin();
  for (; (it != tables_.end()) || ((*it).second == name); ++it)
  {}
  if (it != tables_.end())
  {
    table = (*it).first;
    tables_.erase(it);
  }
  return table;
}

std::string events::ComputerClub::getClientFromQueue()
{
  std::string clientName = std::move(waitingClients_.front());
  waitingClients_.pop();
  return clientName;
}

void events::ComputerClub::processEvent(const Event& event)
{
  currentTime_ = event.getTime();
  event.process(*this);
}
