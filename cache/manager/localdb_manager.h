#pragma once

#include <atomic>
#include <string>
#include <vector>

#include "flare/base/never_destroyed.h"
#include "thirdparty/leveldb/db.h"
#include "cache/model/request_model.h"

namespace zhi {
namespace cache {

class LocaldbManager {
 public:
  ~LocaldbManager() {
    if (_db != nullptr) {
      delete _db;
    }
  };
  static LocaldbManager* Instance() {
    static flare::NeverDestroyedSingleton<LocaldbManager> instance;
    return instance.Get();
  }
  bool Init(std::string&& _path);
  bool Query();
  void Add(std::string&& key, std::string&& value);
  void Batch(const std::vector<InsertRequest>& insert_req_vec);

 private:
  friend class flare::NeverDestroyedSingleton<LocaldbManager>;
  std::atomic_bool _inited;
  std::string _path;
  leveldb::DB* _db;

  LocaldbManager() : _inited(false){};
};

}  // namespace cache
}  // namespace zhi
