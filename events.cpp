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

bool events::Time::operator>=(const Time& other) const
{
  return !(*this < other);
}

bool events::Time::operator<=(const Time& other) const
{
  return !(*this > other);
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
