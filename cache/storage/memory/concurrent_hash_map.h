#pragma once

#include <mutex>
#include <shared_mutex>
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
class HashKv {
 public:
  bool Init(int partition_num);

  V* Get(const std::string& key);

  V* GetOrCreate(std::string&& key, V&& value);

  bool Add(std::string&& key, V&& value);

  bool Del(const std::string& key);

  HashKv() = default;
  ~HashKv() = default;

 private:
  size_t Hash(const std::string& str) const;

  size_t _partition_num;
  std::vector<ConcurrentMap<V>> _map_vec;
};

template <class V>
size_t HashKv<V>::Hash(const std::string& str) const {
  return std::hash<std::string>()(str) % _partition_num;
}

template <class V>
bool HashKv<V>::Init(int partition_num) {
  _partition_num = partition_num;
  _map_vec = std::vector<ConcurrentMap<V>>(partition_num);
  return true;
}

template <class V>
V* HashKv<V>::Get(const std::string& key) {
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
V* HashKv<V>::GetOrCreate(std::string&& key, V&& v) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  auto iter = sub_map.map.try_emplace(std::move(key), std::move(v)).first;
  return &(iter->second);
}

template <class V>
bool HashKv<V>::Add(std::string&& key, V&& value) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.insert_or_assign(std::move(key), std::move(value));
  return true;
}

template <class V>
bool HashKv<V>::Del(const std::string& key) {
  size_t idx = Hash(key);
  auto& sub_map = _map_vec[idx];
  std::unique_lock lock(sub_map.m);
  sub_map.map.erase(key);
  return true;
}

}  // namespace cache
}  // namespace zhi
