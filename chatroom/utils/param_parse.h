#ifndef PARAM_PARSE_H_
#define PARAM_PARSE_H_

#include <string>
#include <optional>

namespace chatroom {
class ParamParse {
 public:
  ParamParse(const std::string& uri) : _uri(uri) {}
  std::optional<std::string> Get(const std::string& key);

 private:
  std::string _uri;
};

}  // namespace chatroom

#endif  // PARAM_PARSE_H_