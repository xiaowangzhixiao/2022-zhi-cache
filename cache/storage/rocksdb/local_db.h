#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "cache/storage/rocksdb/local_db_option.h"
#include "thirdparty/rocksdb/db.h"

namespace zhi {
namespace cache {

// rocksdb的API
// 暂时只支持value为单列
class LocalDB {
 public:
  enum Column {
    COLUMN_DEFAULT,
  };

  using OptionMap = std::map<LocalDB::Column, LocalDbOption>;

  bool OpenDB(const LocalDbOption& op,
              const OptionMap& column_options = OptionMap());

  void CloseDB();

  std::shared_ptr<rocksdb::ColumnFamilyHandle> GetColumnHandle(Column column);

  void ClearColumn(Column column);

 public:
  rocksdb::Status Write(const std::string& key, const std::string& value);

  rocksdb::Status Write(const rocksdb::Slice& key, const rocksdb::Slice& value,
                        Column column);

  rocksdb::Status Read(const rocksdb::Slice& key, std::string* value);

  rocksdb::Status Read(const rocksdb::Slice& key, std::string* value,
                       Column column);

  std::unique_ptr<rocksdb::Iterator> GetIterator(Column column);

  std::unique_ptr<rocksdb::Iterator> GetLast(Column column);

 private:
  void OpenDbAndCreateColumns(const rocksdb::Options& options,
                              const std::string& manifest_path,
                              const LocalDB::OptionMap& column_options);

 private:
  rocksdb::DB* db_;
  std::vector<std::shared_ptr<rocksdb::ColumnFamilyHandle>> handles_;
};

}  // namespace cache
}  // namespace zhi
