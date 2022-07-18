#ifndef PARAM_PARSE_H_
#define PARAM_PARSE_H_

#include <string>
#include <optional>
#include <regex>
#include <unordered_map>

namespace cache {
class ParamParse {
 public:
  using PathParam = std::vector<std::pair<std::string, std::optional<std::string>>>;
  using PathParamMap = std::unordered_map<std::string, std::optional<std::string>>;

  ParamParse(const std::string& uri) : _uri(uri) {}
  std::optional<std::string> Get(const std::string& key);
  /**
   * eg. /room/{roomId}/enter
   **/ 
  PathParamMap GetPathParam(const std::string& path);

 private:
  const std::string& _uri;
};

}  // namespace cache

#endif  // PARAM_PARSE_H_