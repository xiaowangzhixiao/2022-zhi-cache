#pragma once

#include<string>
#include<mutex>
#include<unordered_map>
#include<vector>

#include "flare/base/never_destroyed.h"

namespace zhi {
namespace cache {
class KvManager {
 public:
  KvManager* Instance();

  bool Init(size_t partition_num);

  std::string Query(const std::string& key) const;

  bool Add(const std::string& key, const std::string value);

  bool Del(const std::string& key);

  ~KvManager() = default;

 private:
  friend class flare::NeverDestroyedSingleton<KvManager>;
  KvManager() = default;
  size_t hash(const std::string& str) const;

  size_t _partition_num;
  std::vector<std::unordered_map<std::string, std::string>> _map_vec;
  std::vector<std::mutex> _mutex_vec;
};
}
}
