#pragma once

#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

namespace zhi {
namespace cache {
class ParamParse {
 public:
  using PathParam =
      std::vector<std::pair<std::string, std::optional<std::string>>>;
  using PathParamMap =
      std::unordered_map<std::string, std::optional<std::string>>;

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

}  // namespace zhi
