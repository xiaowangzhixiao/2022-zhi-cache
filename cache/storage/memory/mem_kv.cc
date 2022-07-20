#include "cache/storage/memory/mem_kv.h"

#include <mutex>
#include <string>

namespace zhi {
namespace cache {

size_t MemKv::Hash(const std::string& str) const {
  size_t result = 0;
  for (auto it = str.cbegin(); it != str.cend(); ++it) {
    result = (result * 131) % _partition_num + *it;
  }
  return result % _partition_num;
}

bool MemKv::Init(int partition_num) {
  _partition_num = partition_num;
  _map_vec = std::vector<ConcurrentMap>(partition_num);
  return true;
}

const std::string* MemKv::Query(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::shared_lock lock(sub_map.m);
  auto it = sub_map.map.find(key);
  if (it != sub_map.map.end()) {
    return &(it->second);
  }
  return nullptr;
}

bool MemKv::Add(std::string&& key, std::string&& value) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.insert_or_assign(std::move(key), std::move(value));
  return true;
}

bool MemKv::Del(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.erase(key);
  return true;
}
}  // namespace cache
}  // namespace zhi
