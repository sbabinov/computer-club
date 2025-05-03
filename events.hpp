#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <ostream>
#include <iomanip>
#include <string>

namespace events
{
  struct Time
  {
    unsigned short hours;
    unsigned short minutes;

    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
  };
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
    virtual void process(ComputerClub& club) = 0;
  protected:
    Time time_;
  };

  class ClientEvent: public Event
  {
  public:
    ClientEvent(Time time, const std::string& clientName);
  protected:
    std::string clientName_;
  };

  class ClientCameEvent: public ClientEvent
  {
  public:
    ClientCameEvent(Time time, const std::string& clientName);
    virtual void process(ComputerClub& club) override {};
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
    virtual void process(ComputerClub& club) override {};
  private:
    std::string clientName_;
    Type type_;
  };

  class ClientWaitingEvent: public ClientEvent
  {
  public:
    ClientWaitingEvent(Time time, const std::string& clientName);
    virtual void process(ComputerClub& club) override {};
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
    virtual void process(ComputerClub& club) override {};
  private:
    std::string clientName_;
    Type type_;
  };

  class ErrorEvent: public Event
  {
  public:
    ErrorEvent(Time time, ErrorType error);
    virtual void process(ComputerClub& club) override {};
  private:
    ErrorType error_;
  };
}

#endif
