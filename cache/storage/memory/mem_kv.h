#pragma once

#include <shared_mutex>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace zhi {
namespace cache {

template <class V>
struct ConcurrentMap {
  std::shared_mutex m;
  std::unordered_map<std::string, V> map;
};

template <class V>
class MemKv {
 public:
  bool Init(int partition_num);

  const V* Query(const std::string& key);

  bool Add(std::string&& key, V&& value);

  bool Del(const std::string& key);

  MemKv() = default;
  ~MemKv() = default;

 private:
  size_t Hash(const std::string& str) const;

  size_t _partition_num;
  std::vector<ConcurrentMap<V>> _map_vec;
};

template <class V>
size_t MemKv<V>::Hash(const std::string& str) const {
  size_t result = 0;
  for (auto it = str.cbegin(); it != str.cend(); ++it) {
    result = (result * 131) % _partition_num + *it;
  }
  return result % _partition_num;
}

template <class V>
bool MemKv<V>::Init(int partition_num) {
  _partition_num = partition_num;
  _map_vec = std::vector<ConcurrentMap<V>>(partition_num);
  return true;
}

template <class V>
const V* MemKv<V>::Query(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::shared_lock lock(sub_map.m);
  auto it = sub_map.map.find(key);
  if (it != sub_map.map.end()) {
    return &(it->second);
  }
  return nullptr;
}

template <class V>
bool MemKv<V>::Add(std::string&& key, V&& value) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  // std::unique_lock lock(sub_map.m);
  sub_map.map.insert_or_assign(std::move(key), std::move(value));
  return true;
}

template <class V>
bool MemKv<V>::Del(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.erase(key);
  return true;
}

}  // namespace cache
}  // namespace zhi
