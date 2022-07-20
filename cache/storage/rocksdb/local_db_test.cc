#include "cache/storage/rocksdb/local_db.h"

#include <filesystem>

#include "gtest/gtest.h"
#include "thirdparty/glog/logging.h"

namespace zhi {
namespace cache {

namespace fs = std::filesystem;

TEST(LocalDB, WriteAndRead) {
  LocalDbOption db_option;
  db_option.path = "./roc_db";
  fs::path path = db_option.path;
  if (!fs::exists(path)) {
    fs::create_directory(path);
  }

  std::map<LocalDB::Column, LocalDbOption> column_options;
  auto local_db_option = LocalDbOption();
  local_db_option.ttl = 3600 * 24;
  column_options[LocalDB::COLUMN_DEFAULT] = local_db_option;
  LocalDB localdb;
  localdb.OpenDB(db_option, column_options);
  LOG(INFO) << "Init localdb!";

  EXPECT_EQ(::rocksdb::Status::OK(),
            localdb.Write("key", "value", LocalDB::COLUMN_DEFAULT));
  std::string value;
  EXPECT_EQ(::rocksdb::Status::OK(),
            localdb.Read("key", &value, LocalDB::COLUMN_DEFAULT));
  EXPECT_EQ("value", value);

  EXPECT_EQ(::rocksdb::Status::OK(), localdb.Write("key2", "value2"));
  std::string value2;
  EXPECT_EQ(::rocksdb::Status::OK(), localdb.Read("key2", &value2));
  EXPECT_EQ("value2", value2);
  localdb.CloseDB();
}

}  // namespace cache
}  // namespace zhi
