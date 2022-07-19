#include "cache/manager/kv_manager.h"

#include <mutex>
#include <string>

namespace zhi {
namespace cache {

size_t KvManager::Hash(const std::string& str) const {
  size_t result = 0;
  for (auto it = str.cbegin(); it != str.cend(); ++it) {
    result = (result * 131) % _partition_num + *it;
  }
  return result % _partition_num;
}

KvManager* KvManager::Instance() {
  static flare::NeverDestroyedSingleton<KvManager> instance;
  return instance.Get();
}

bool KvManager::Init() {
  _partition_num = 1024;
  _map_vec = std::vector<ConcurrentMap>(1024);
  return true;
}

const std::string* KvManager::Query(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::shared_lock lock(sub_map.m);
  auto it = sub_map.map.find(key);
  if (it != sub_map.map.end()) {
    return &(it->second);
  }
  return nullptr;
}

bool KvManager::Add(std::string&& key, std::string&& value) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.insert_or_assign(std::move(key), std::move(value));
  return true;
}

bool KvManager::Del(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.erase(key);
  return true;
}
}  // namespace cache
}  // namespace zhi
