#include "guards.hpp"

guards::ScopeGuard::ScopeGuard(std::basic_ios< char >& s):
  s_(s),
  precision_(s.precision()),
  flags_(s.flags())
{}

guards::ScopeGuard::~ScopeGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
}
