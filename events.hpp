#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <iostream>
#include <string>
#include <memory>
#include "structs.hpp"
#include "club.hpp"

namespace club
{
  class ComputerClub;
}

namespace events
{
  using structs::Time;

  enum class EventType
  {
    INCOMING,
    OUTCOMING
  };

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
    Event() = default;
    Event(Time time, EventType type);
    virtual ~Event() = default;
    Time getTime() const;
    virtual void process(club::ComputerClub& club) const;
    virtual void print(std::ostream& out) const;
  protected:
    unsigned short id_;
    EventType type_;
    Time time_;
  };
  std::ostream& operator<<(std::ostream& out, const Event& event);

  class ClientEvent: public Event
  {
  public:
    ClientEvent() = default;
    ClientEvent(Time time, EventType type, const std::string& clientName);
    virtual void print(std::ostream& out) const override;
  protected:
    std::string clientName_;
  };
  std::istream& operator>>(std::istream& in, std::unique_ptr< ClientEvent >& event);

  class ClientCameEvent: public ClientEvent
  {
  public:
    ClientCameEvent(Time time, const std::string& clientName);
    virtual void process(club::ComputerClub& club) const override;
  };

  class ClientSatEvent: public ClientEvent
  {
  public:
    ClientSatEvent(Time time, const std::string& clientName, size_t table, EventType type);
    virtual void print(std::ostream& out) const override;
    virtual void process(club::ComputerClub& club) const override;
  private:
    size_t table_;
  };

  class ClientWaitingEvent: public ClientEvent
  {
  public:
    ClientWaitingEvent(Time time, const std::string& clientName);
    virtual void process(club::ComputerClub& club) const override;
  };

  class ClientLeftEvent: public ClientEvent
  {
  public:
    ClientLeftEvent(Time time, const std::string& clientName, EventType type);
    virtual void process(club::ComputerClub& club) const override;
  };

  class ErrorEvent: public Event
  {
  public:
    ErrorEvent(Time time, ErrorType error);
    virtual void print(std::ostream& out) const override;
  private:
    ErrorType error_;
  };
}

#endif
