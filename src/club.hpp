#ifndef CLUB_HPP
#define CLUB_HPP
#include <set>
#include <queue>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include "structs.hpp"
#include "events.hpp"

namespace events
{
  class Event;
  class ClientEvent;
}

namespace club
{
  using structs::Time;

  class ComputerClub
  {
  public:
    ComputerClub(size_t nTables, std::pair< Time, Time > workingTime, size_t price, std::ostream* logStream);
    ~ComputerClub();

    bool isOpen() const;
    bool hasClient(const std::string& name) const;
    bool hasAvailableTable() const;
    bool isTableOccupied(size_t table) const;
    bool isQueueEmpty() const;
    bool isQueueFull() const;
    Time currentTime() const;
    void addClient(const std::string& name);
    void addClientToQueue(const std::string& name);
    void assignTable(const std::string& name, size_t table);
    size_t removeClient(const std::string& name);
    std::string getClientFromQueue();

    void logEvent(const events::Event& event);
    void processEvent(const events::ClientEvent& event);
  private:
    std::set< std::string > clients_;
    std::queue< std::string > waitingClients_;
    std::map< size_t, std::pair< std::string, Time > > tables_;
    std::map< size_t, std::pair< size_t, Time > > tableRevenue_;
    size_t nTables_;
    size_t price_;
    std::pair< Time, Time > workingTime_;
    Time currentTime_;
    std::ostream* logStream_;

    size_t updateRevenue(const std::string& name);
  };
}

#endif
