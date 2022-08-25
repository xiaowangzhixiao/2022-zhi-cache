
#include "localdb_manager.h"

#include <atomic>
#include <string>

#include "flare/base/logging.h"
#include "thirdparty/leveldb/db.h"
#include "cache/manager/kv_manager.h"
#include "leveldb/write_batch.h"

namespace zhi {
namespace cache {

bool LocaldbManager::Init(std::string&& path) {
  _path = std::move(path);
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, _path, &_db);
  if (!status.ok()) {
    LOG(ERROR) << "init db error";
    return false;
  }
  leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    KvManager::Instance()->Add(it->key().ToString(), it->value().ToString());
  }
  delete it;
  _inited = true;
  return true;
}

bool LocaldbManager::Query() {
  return _inited.load();
}

void LocaldbManager::Add(std::string&& key, std::string&& value) {
  leveldb::WriteOptions write_option;
  _db->Put(write_option, std::move(key), std::move(value));
}

void LocaldbManager::Batch(const std::vector<InsertRequest>& insert_req_vec) {
  leveldb::WriteOptions write_option;
  leveldb::WriteBatch write_batch;
  for (auto insert_request : insert_req_vec) {
    write_batch.Put(std::move(insert_request.key), std::move(insert_request.value));
  }
  _db->Write(write_option, &write_batch);
}

}  // namespace cache
}  // namespace zhi
