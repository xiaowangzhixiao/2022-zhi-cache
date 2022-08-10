#pragma once

#include <string>
#include <vector>

#include "cache/storage/memory/concurrent_hash_map.h"
#include "cache/storage/memory/zset.h"
#include "flare/base/never_destroyed.h"
#include "cache/utils/arena.h"

namespace zhi {
namespace cache {

class ZsetManager {
 public:
  ~ZsetManager() = default;
  static ZsetManager* Instance() {
    static flare::NeverDestroyedSingleton<ZsetManager> instance;
    return instance.Get();
  }
  bool Init();
  bool Add(std::string&& key, int32_t score, std::string&& value);
  std::pair<bool, std::vector<ZaddRequest>> Range(const std::string& key,
                                                  int32_t min_score,
                                                  int32_t max_score);
  bool Del(const std::string& key, const std::string& value);

 private:
  friend class flare::NeverDestroyedSingleton<ZsetManager>;
  ZsetManager() : _arena(std::make_shared<Arena>()){};
  HashKv<std::shared_ptr<Zset>> _zset_map;
  std::shared_ptr<Arena> _arena;
};

}  // namespace cache
}  // namespace zhi
