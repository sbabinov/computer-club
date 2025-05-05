#include "club.hpp"
#include <algorithm>

size_t calculate_hours_dif(structs::Time moreTime, structs::Time lessTime)
{
  structs::Time dif = moreTime - lessTime;
  return dif.hours + ((dif.minutes != 0) ? 1 : 0);
}

club::ComputerClub::ComputerClub(size_t nTables, std::pair< club::Time, club::Time > workingTime,
 size_t price, std::ostream* logStream):
  nTables_(nTables),
  price_(price),
  workingTime_(workingTime),
  currentTime_(),
  tables_(),
  tableRevenue_(),
  clients_(),
  waitingClients_(),
  logStream_(logStream)
{
  for (size_t i = 0; i < nTables_; ++i)
  {
    tableRevenue_[i + 1] = {0, Time{0, 0}};
  }
  (*logStream_) << workingTime_.first << '\n';
}

club::ComputerClub::~ComputerClub()
{
  currentTime_ = workingTime_.second;
  for (auto it = clients_.begin(); it != clients_.end(); ++it)
  {
    events::ClientLeftEvent(currentTime_, *it, events::EventType::OUTCOMING).process(*this);
  }
  (*logStream_) << workingTime_.second << '\n';
  for (auto it = tableRevenue_.begin(); it != tableRevenue_.end(); ++it)
  {
    (*logStream_) << it->first << ' ' << (it->second).first << ' ' << (it->second).second << '\n';
  }
}

bool club::ComputerClub::isOpen() const
{
  return (workingTime_.first <= currentTime_) && (currentTime_ < workingTime_.second);
}

bool club::ComputerClub::hasClient(const std::string& name) const
{
  return clients_.find(name) != clients_.cend();
}

bool club::ComputerClub::hasAvailableTable() const
{
  return tables_.size() < nTables_;
}

bool club::ComputerClub::isTableOccupied(size_t table) const
{
  return tables_.find(table) != tables_.cend();
}

bool club::ComputerClub::isQueueEmpty() const
{
  return waitingClients_.empty();
}

structs::Time club::ComputerClub::currentTime() const
{
  return currentTime_;
}

void club::ComputerClub::addClient(const std::string& name)
{
  clients_.insert(name);
}

void club::ComputerClub::addClientToQueue(const std::string& name)
{
  waitingClients_.push(name);
}

void club::ComputerClub::assignTable(const std::string& name, size_t table)
{
  tables_[table] = {name, currentTime_};
}

size_t club::ComputerClub::removeClient(const std::string& name)
{
  size_t table = 0;
  auto it = tables_.begin();
  for (; (it != tables_.end()) && ((*it).second.first != name); ++it)
  {}
  if (it != tables_.end())
  {
    table = (*it).first;
    Time occupationTime = currentTime_ - (it->second).second;
    tableRevenue_[table].first += calculate_hours_dif(currentTime_, (it->second).second) * price_;
    tableRevenue_[table].second = tableRevenue_[table].second + occupationTime;
    tables_.erase(it);
  }
  if (isOpen())
  {
    clients_.erase(name);
  }
  return table;
}

std::string club::ComputerClub::getClientFromQueue()
{
  std::string clientName = std::move(waitingClients_.front());
  waitingClients_.pop();
  return clientName;
}

void club::ComputerClub::logEvent(const events::Event& event)
{
  (*logStream_) << event << '\n';
}

void club::ComputerClub::processEvent(const events::ClientEvent& event)
{
  currentTime_ = event.getTime();
  event.process(*this);
}
