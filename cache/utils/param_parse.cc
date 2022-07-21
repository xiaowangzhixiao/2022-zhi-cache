#include "param_parse.h"

#include <iostream>
#include <regex>

namespace zhi {
namespace cache {
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

ParamParse::PathParamMap ParamParse::GetPathParam(const std::string& path) {
  PathParam path_param;
  std::smatch param_key_result;
  auto path_iter = path.begin();
  while (std::regex_search(path_iter, path.end(), param_key_result,
                           std::regex("\\{(.*?)\\}"))) {
    path_param.emplace_back(std::move(param_key_result[1]), std::nullopt);
    path_iter = param_key_result.suffix().first;
  }

  std::string regex =
      std::regex_replace(path, std::regex("\\{(.*?)\\}"), "(.*?)");
  std::smatch param_value_result;
  if (std::regex_search(_uri.cbegin(), _uri.cend(), param_value_result,
                        std::regex(regex))) {
    for (size_t i = 1;
         i < param_value_result.size() && i - 1 < path_param.size(); ++i) {
      path_param[i - 1].second = param_value_result[i];
    }
  }

  return PathParamMap(path_param.begin(), path_param.end());
}
}  // namespace cache

}  // namespace zhi
