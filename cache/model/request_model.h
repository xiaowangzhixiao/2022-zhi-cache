#pragma once
#include <string>

#include "thirdparty/xpack/json.h"

namespace zhi {
namespace cache {

struct InsertRequest {
  std::string key;
  std::string value;
  XPACK(O(key, value));
};

struct ZaddRequest
{
  int32_t score;
  std::string value;
  XPACK(O(score, value));
};

struct ZrangeRequest
{
  int32_t min_score;
  int32_t max_score;
  XPACK(O(min_score, max_score));
};

}  // namespace cache
}  // namespace zhi
