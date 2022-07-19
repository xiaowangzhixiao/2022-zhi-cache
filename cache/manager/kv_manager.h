#pragma once

#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "flare/base/never_destroyed.h"

namespace zhi {
namespace cache {

struct ConcurrentMap {
  std::shared_mutex m;
  std::unordered_map<std::string, std::string> map;
};

class KvManager {
 public:
  KvManager* Instance();

  bool Init();

  const std::string* Query(const std::string& key);

  bool Add(std::string&& key, std::string&& value);

  bool Del(const std::string& key);

  ~KvManager() = default;

 private:
  friend class flare::NeverDestroyedSingleton<KvManager>;
  KvManager() = default;
  size_t Hash(const std::string& str) const;

  size_t _partition_num;
  std::vector<ConcurrentMap> _map_vec;
};

}  // namespace cache
}  // namespace zhi
