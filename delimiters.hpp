#ifndef DELIMITERS_HPP
#define DELIMITERS_HPP
#include <istream>

namespace delimiters
{
  struct CharDelimiter
  {
    char expected;
  };
  std::istream& operator>>(std::istream& in, CharDelimiter&& del);
}

#endif
