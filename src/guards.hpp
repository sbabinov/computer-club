#ifndef GUARDS_HPP
#define GUARDS_HPP
#include <ios>

namespace guards
{
  class ScopeGuard
  {
  public:
    explicit ScopeGuard(std::basic_ios< char >& c);
    ~ScopeGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
  };
}

#endif
