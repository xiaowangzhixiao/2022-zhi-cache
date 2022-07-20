#include "cache/manager/kv_manager.h"

#include <mutex>
#include <string>

namespace zhi {
namespace cache {

KvManager* KvManager::Instance() {
  static flare::NeverDestroyedSingleton<KvManager> instance;
  return instance.Get();
}

bool KvManager::Init() {
  _memdb.Init(1024);
  return true;
}

const std::string* KvManager::Query(const std::string& key) {
  return _memdb.Query(key);
}

bool KvManager::Add(std::string&& key, std::string&& value) {
  return _memdb.Add(std::move(key), std::move(value));
}

bool KvManager::Del(const std::string& key) {
  return _memdb.Del(key);
}
}  // namespace cache
}  // namespace zhi
