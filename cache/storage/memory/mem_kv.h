#pragma once

#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace zhi {
namespace cache {

struct ConcurrentMap {
  std::shared_mutex m;
  std::unordered_map<std::string, std::string> map;
};

class MemKv {
 public:
  bool Init(int partition_num);

  const std::string* Query(const std::string& key);

  bool Add(std::string&& key, std::string&& value);

  bool Del(const std::string& key);

  MemKv() = default;
  ~MemKv() = default;

 private:
  size_t Hash(const std::string& str) const;

  size_t _partition_num;
  std::vector<ConcurrentMap> _map_vec;
};

}  // namespace cache
}  // namespace zhi
