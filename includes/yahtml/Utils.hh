#ifndef YAHTML__UTILS__UTILS_HH
#define YAHTML__UTILS__UTILS_HH 1

#include <vector>
#include <string>
#include <algorithm>

namespace yahtml { namespace utils {

inline std::vector<std::string> split_str (const std::string& str,
                                           int delimiter(int) = ::isspace)
{
  std::vector<std::string> result;
  std::string::const_iterator end = str.end();
  std::string::const_iterator i = str.begin();
  std::string::const_iterator j;

  while (i != end) {
    i = std::find_if_not(i, end, delimiter);
    if (i == end)
      break;

    j = find_if(i, end, delimiter);
    result.push_back(std::string(i, j));
    i = j;
  }

  return result;
}

}}; // ! ns yahtml utils

#endif
