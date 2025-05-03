#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <queue>
#include <map>
#include <utility>

namespace events
{
  struct Time
  {
    unsigned short hours;
    unsigned short minutes;

    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator<=(const Time& other) const;
  };
  std::istream& operator>>(std::istream& in, Time& time);
  std::ostream& operator<<(std::ostream& out, const Time& time);

  class ComputerClub;

  enum class ErrorType
  {
    YOU_SHALL_NOT_PASS,
    NOT_OPEN_YET,
    CLIENT_UNKNOWN,
    PLACE_IS_BUSY,
    I_CAN_WAIT_NO_LONGER
  };

  class Event
  {
  public:
    Event(Time time);
    virtual ~Event() = default;
    Time getTime() const;
    virtual void process(ComputerClub& club) const = 0;
    friend std::ostream& operator<<(std::ostream& out, const Event& event);
  protected:
    unsigned short id_;
    Time time_;
  };
  std::istream& operator>>(std::istream& in, Event& event);

  class ClientEvent: public Event
  {
  public:
    ClientEvent(Time time, const std::string& clientName);
    friend std::ostream& operator<<(std::ostream& out, const ClientEvent& event);
  protected:
    std::string clientName_;
  };

  class ClientCameEvent: public ClientEvent
  {
  public:
    ClientCameEvent(Time time, const std::string& clientName);
    virtual void process(ComputerClub& club) const override {};
  private:
    std::string clientName_;
  };

  struct ClientSatEvent: public ClientEvent
  {
  public:
    enum class Type
    {
      INCOMING,
      OUTCOMING
    };
    ClientSatEvent(Time time, const std::string& clientName, Type type);
    virtual void process(ComputerClub& club) const override {};
  private:
    std::string clientName_;
    Type type_;
  };

  class ClientWaitingEvent: public ClientEvent
  {
  public:
    ClientWaitingEvent(Time time, const std::string& clientName);
    virtual void process(ComputerClub& club) const override {};
  private:
    std::string clientName_;
  };

  struct ClientLeftEvent: public ClientEvent
  {
  public:
    enum class Type
    {
      INCOMING,
      OUTCOMING
    };
    ClientLeftEvent(Time time, const std::string& clientName, Type type);
    virtual void process(ComputerClub& club) const override {};
  private:
    std::string clientName_;
    Type type_;
  };

  class ErrorEvent: public Event
  {
  public:
    ErrorEvent(Time time, ErrorType error);
    virtual void process(ComputerClub& club) const override {};
    friend std::ostream& operator<<(std::ostream& out, const ErrorEvent& event);
  private:
    ErrorType error_;
  };

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

    void processEvent(const Event& event);
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
