#pragma once

#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "cache/storage/memory/concurrent_hash_map.h"
#include "flare/base/never_destroyed.h"

namespace zhi {
namespace cache {

class KvManager {
 public:
  static KvManager* Instance();

  bool Init();

  const std::string* Query(const std::string& key);

  bool Add(std::string key, std::string value);

  bool Del(const std::string& key);

  ~KvManager() = default;

 private:
  friend class flare::NeverDestroyedSingleton<KvManager>;
  KvManager() = default;
  HashKv<std::string> _memdb;
};

}  // namespace cache
}  // namespace zhi
