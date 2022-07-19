#include "cache/manager/kv_manager.h"

#include<mutex>
#include<string>

namespace zhi {
namespace cache {

size_t KvManager::Hash(const std::string& str) const {
  size_t result = 0;
  for(auto it = str.cbegin(); it != str.cend(); ++it) {
    result = (result * 131) % _partition_num + *it;
  }
  return result % _partition_num;
}

KvManager* KvManager::Instance() {
  static flare::NeverDestroyedSingleton<KvManager> instance;
  return instance.Get();
}


bool KvManager::Init(size_t partition_num) {
  _partition_num = partition_num;
  _map_vec = std::vector<std::unordered_map>(partition_num);
  _mutex_vec = std::vector<std::mutex>(partition_num);
}


std::string* KvManager::Query(const std::string& key) const {
  size_t idx = Hash(key);
  std::scoped_lock lock(_mutex_vec[idx]);
  auto it = _map_vec[idx].find(key);
  if (it != _map_vec[idx].end()) {
    return &(*it);
  }
  return nullptr;
}


bool kvManager::Add(std::string&& key, std::string&& value) {
  size_t idx = Hash(key);
  std::scoped_lock lock(_mutex_vec[idx]);
  _map_vec[idx].insert_or_assign(std::move(key), std::move(value));
  return true;
}

bool Del(const std::string& key);
  size_t idx = Hash(key);
  std::scoped_lock lock(_mutex_vec[idx]);
  _map_vec[idx].erase(key);
  return true;
}
}
