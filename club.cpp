#include "club.hpp"

club::ComputerClub::ComputerClub(size_t nTables, std::pair< club::Time, club::Time > workingTime,
 size_t price, std::ostream* logStream):
  nTables_(nTables),
  price_(price),
  workingTime_(workingTime),
  currentTime_(),
  tables_(),
  clients_(),
  waitingClients_(),
  logStream_(logStream)
{}

bool club::ComputerClub::isOpen() const
{
  return (workingTime_.first <= currentTime_) && (currentTime_ <= workingTime_.second);
}

bool club::ComputerClub::hasClient(const std::string& name) const
{
  return clients_.find(name) != clients_.cend();
}

bool club::ComputerClub::hasAvailableTable() const
{
  return nTables_ < clients_.size();
}

bool club::ComputerClub::isTableOccupied(size_t table) const
{
  return tables_.find(table) != tables_.cend();
}

bool club::ComputerClub::isQueueEmpty() const
{
  return waitingClients_.empty();
}

void club::ComputerClub::addClient(const std::string& name)
{
  clients_.insert(name);
}

void club::ComputerClub::assignTable(const std::string& name, size_t table)
{
  tables_[table] = name;
}

size_t club::ComputerClub::removeClient(const std::string& name)
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

std::string club::ComputerClub::getClientFromQueue()
{
  std::string clientName = std::move(waitingClients_.front());
  waitingClients_.pop();
  return clientName;
}

void club::ComputerClub::logEvent(const events::Event& event)
{
  (*logStream_) << event;
}

void club::ComputerClub::processEvent(const events::Event& event)
{
  currentTime_ = event.getTime();
  event.process(*this);
}
