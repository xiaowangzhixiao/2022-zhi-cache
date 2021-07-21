#include "param_parse.h"

#include <regex>

namespace chatroom {
std::optional<std::string> ParamParse::Get(const std::string& key) {
  std::smatch result;
  if (std::regex_search(_uri.cbegin(), _uri.cend(), result,
                        std::regex(key + "=(.*?)&"))) {
    return result[1];
  } else if (std::regex_search(_uri.cbegin(), _uri.cend(), result,
                               std::regex(key + "=(.*)"))) {
    return result[1];
  } else {
    return std::nullopt;
  }
}
}  // namespace chatroom