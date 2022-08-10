#include "cache/manager/zset_manager.h"

namespace zhi {
namespace cache {

bool ZsetManager::Init() {
  _zset_map.Init(87719);
  return true;
}

bool ZsetManager::Add(std::string&& key, int32_t score, std::string&& value) {
  auto* zset = _zset_map.Get(key);
  if (!zset) {
    zset =
        _zset_map.GetOrCreate(std::move(key), std::make_shared<Zset>(_arena));
  }
  return zset->get()->Add(score, value);
}

std::pair<bool, std::vector<ZaddRequest>> ZsetManager::Range(
    const std::string& key, int32_t min_score, int32_t max_score) {
  auto* zset = _zset_map.Get(key);
  if (!zset) {
    return std::make_pair<bool, std::vector<ZaddRequest>>(false, {});
  }
  return {true, zset->get()->Range(min_score, max_score)};
}

bool ZsetManager::Del(const std::string& key, const std::string& value) {
  auto* zset = _zset_map.Get(key);
  if (!zset) {
    return true;
  }
  return zset->get()->Del(value);
}
}  // namespace cache

}  // namespace zhi
