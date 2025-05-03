#include "delimiter.hpp"
#include <ios>

std::istream& delimiters::operator>>(std::istream& in, delimiters::CharDelimiter&& del)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (c != del.expected)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
