#ifndef CLUB_HPP
#define CLUB_HPP
#include <set>
#include <queue>
#include <map>
#include <string>
#include <utility>
#include "structs.hpp"
#include "events.hpp"

namespace events
{
  class Event;
}

namespace club
{
  using structs::Time;

  class ComputerClub
  {
  public:
    ComputerClub(size_t nTables, std::pair< Time, Time > workingTime, size_t price);
    bool isOpen() const;
    bool hasClient(const std::string& name) const;
    bool hasAvailableTable() const;
    bool isTableOccupied(size_t table) const;
    bool isQueueEmpty() const;
    void addClient(const std::string& name);
    void assignTable(const std::string& name, size_t table);
    size_t removeClient(const std::string& name);
    std::string getClientFromQueue();

    void processEvent(const events::Event& event);
  private:
    std::set< std::string > clients_;
    std::queue< std::string > waitingClients_;
    std::map< size_t, std::string > tables_;
    size_t nTables_;
    size_t price_;
    std::pair< Time, Time > workingTime_;
    Time currentTime_;
  };
}

#endif
