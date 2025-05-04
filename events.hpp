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
    Event(Time time);
    virtual ~Event() = default;
    Time getTime() const;
    virtual void process(club::ComputerClub& club) const;
    virtual void print(std::ostream& out) const;
  protected:
    unsigned short id_;
    Time time_;
  };
  std::ostream& operator<<(std::ostream& out, const Event& event);

  class ClientEvent: public Event
  {
  public:
    ClientEvent() = default;
    ClientEvent(Time time, const std::string& clientName);
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
    enum class Type
    {
      INCOMING,
      OUTCOMING
    };
    ClientSatEvent(Time time, const std::string& clientName, size_t table, Type type);
    virtual void print(std::ostream& out) const override;
    virtual void process(club::ComputerClub& club) const override;
  private:
    size_t table_;
    Type type_;
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
    enum class Type
    {
      INCOMING,
      OUTCOMING
    };
    ClientLeftEvent(Time time, const std::string& clientName, Type type);
    virtual void process(club::ComputerClub& club) const override;
  private:
    Type type_;
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
