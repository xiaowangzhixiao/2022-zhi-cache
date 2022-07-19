// Copyright (c) 2019 Tencent Inc.
// Author: Willie Kou (williekou@tencent.com)

#include "cache/storage/rocksdb/local_db.h"

#include <filesystem>
#include <map>
#include <thread>
#include <utility>

#include "cache/storage/rocksdb/local_db_option.h"
#include "glog/logging.h"
#include "thirdparty/rocksdb/cache.h"
#include "thirdparty/rocksdb/merge_operator.h"
#include "thirdparty/rocksdb/table.h"

namespace zhi {
namespace cache {

namespace fs = std::filesystem;

class CasOperator : public rocksdb::AssociativeMergeOperator {
  bool Merge(const rocksdb::Slice& key, const rocksdb::Slice* existing_value,
             const rocksdb::Slice& value, std::string* new_value,
             rocksdb::Logger* logger) const override {
    if (existing_value == nullptr) {
      new_value->assign(value.data_, value.size_);
      return true;
    }
    if (existing_value->size_ == value.size_ &&
        memcmp(existing_value->data_, value.data_, existing_value->size_) ==
            0) {
      new_value->assign(existing_value->data_, existing_value->size_);
      return true;
    }
    new_value->assign(value.data_, value.size_);
    return true;
  }

  const char* Name() const override { return "CasOperator"; }
};

bool LocalDB::OpenDB(const LocalDbOption& op,
                     const LocalDB::OptionMap& column_options) {
  const std::string& base_path = op.path;
  std::string data_path = base_path + "/data";
  std::string db_log_path = base_path + "/log";
  std::string wal_path = base_path + "/wal";
  std::string manifest_path = base_path + "/manifest";
  LOG(INFO) << "The data_path: " << data_path
            << ", db_log_path: " << db_log_path << ", wal_path: " << wal_path
            << ", manifest_path: " << manifest_path;

  rocksdb::Options options;
  options.create_if_missing = true;
  if (op.write_buffer_size_in_mb > 0) {
    options.write_buffer_size = op.write_buffer_size_in_mb << 20;
  }
  if (op.max_write_buffer_number > 0) {
    options.max_write_buffer_number = op.max_write_buffer_number;
  }
  if (op.min_write_buffer_number_to_merge > 0) {
    options.min_write_buffer_number_to_merge =
        op.min_write_buffer_number_to_merge;
  }
  options.level0_slowdown_writes_trigger = 20;
  options.level0_stop_writes_trigger = 100;
  options.max_background_compactions = op.max_background_compactions;
  options.compaction_readahead_size = op.compaction_readahead_size;
  rocksdb::DbPath db_path;
  db_path.path = data_path;
  // db_path.target_size = uint64_t(200) * 1024 * 1024 * 1024;
  db_path.target_size = uint64_t(1024) * 1024 * 1024 * 1024 * 10;
  options.db_paths.push_back(db_path);
  fs::create_directory(data_path);
  if (!db_log_path.empty()) {
    options.db_log_dir = db_log_path;
    fs::create_directory(db_log_path);
  }
  if (!wal_path.empty()) {
    options.wal_dir = wal_path;
    fs::create_directory(wal_path);
  }
  if (!manifest_path.empty()) {
    fs::create_directory(manifest_path);
  }
  if (op.block_cache_size_in_mb > 0) {
    auto capacity = op.block_cache_size_in_mb << 20;
    std::shared_ptr<rocksdb::Cache> cache = rocksdb::NewLRUCache(capacity);
    rocksdb::BlockBasedTableOptions table_options;
    table_options.block_cache = cache;
    options.table_factory.reset(
        rocksdb::NewBlockBasedTableFactory(table_options));
  } else if (op.block_cache_size_in_mb < 0) {
    rocksdb::BlockBasedTableOptions table_options;
    table_options.no_block_cache = true;
    options.table_factory.reset(
        rocksdb::NewBlockBasedTableFactory(table_options));
  } else {
    rocksdb::BlockBasedTableOptions table_options;
    options.table_factory.reset(
        rocksdb::NewBlockBasedTableFactory(table_options));
  }
  options.dump_malloc_stats = true;
  options.merge_operator.reset(new CasOperator());
  OpenDbAndCreateColumns(options, manifest_path, column_options);
  return db_ != nullptr;
}

static void ConfigColumnOptionString(
    const LocalDbOption& option,
    std::unordered_map<std::string, std::string>* option_strings) {
  (*option_strings)["write_buffer_size"] =
      std::to_string(option.write_buffer_size_in_mb << 20);
  (*option_strings)["max_write_buffer_number"] =
      std::to_string(option.max_write_buffer_number);
  (*option_strings)["ttl"] = std::to_string(option.ttl);
}

static std::map<std::string, LocalDB::Column> InitColumnNames() {
  std::map<std::string, LocalDB::Column> all_cf_names;
  all_cf_names[rocksdb::kDefaultColumnFamilyName] = LocalDB::COLUMN_DEFAULT;
  return all_cf_names;
}

void GetColumns(const rocksdb::Options& options,
                const std::string& manifest_path,
                const std::map<std::string, LocalDB::Column>& all_cf_names,
                std::vector<std::string>* existing_cf_names,
                std::vector<std::string>* missing_cf_name) {
  rocksdb::DB::ListColumnFamilies(options, manifest_path, existing_cf_names);
  if (existing_cf_names->empty()) {
    existing_cf_names->push_back(rocksdb::kDefaultColumnFamilyName);
  }

  for (const auto& iter : all_cf_names) {
    bool existing = false;
    for (const auto& cf_name : *existing_cf_names) {
      if (cf_name == iter.first) {
        LOG(INFO) << "Existing Column Family Name: " << cf_name;
        existing = true;
        break;
      }
    }
    if (!existing) {
      missing_cf_name->push_back(iter.first);
      LOG(WARNING) << "Missing Column Family Name: " << iter.first;
    }
  }
}

void LocalDB::OpenDbAndCreateColumns(const rocksdb::Options& options,
                                     const std::string& manifest_path,
                                     const LocalDB::OptionMap& column_options) {
  std::map<std::string, LocalDB::Column> all_cf_names = InitColumnNames();
  std::vector<std::string> existing_cf_names;
  std::vector<std::string> missing_cf_name;
  GetColumns(options, manifest_path, all_cf_names, &existing_cf_names,
             &missing_cf_name);
  std::vector<rocksdb::ColumnFamilyDescriptor> existing_cf_desc;
  for (const auto& cf_name : existing_cf_names) {
    rocksdb::ColumnFamilyDescriptor cf;
    cf.name = cf_name;
    existing_cf_desc.push_back(cf);
  }

  std::vector<rocksdb::ColumnFamilyHandle*> existing_cf_handles;
  auto status = rocksdb::DB::Open(options, manifest_path, existing_cf_desc,
                                  &existing_cf_handles, &db_);
  if (status.ok()) {
    LOG(INFO) << "DB " << manifest_path << " is open";
  } else {
    CHECK(false) << "Failed to open DB " << manifest_path
                 << ", status=" << status.ToString();
  }

  handles_.resize(all_cf_names.size());
  for (const auto& cf_name : missing_cf_name) {
    rocksdb::ColumnFamilyHandle* new_handle = nullptr;
    auto status = db_->CreateColumnFamily(rocksdb::ColumnFamilyOptions(),
                                          cf_name, &new_handle);
    if (new_handle != nullptr) {
      LOG(INFO) << "DB Column Familiy " << cf_name << " is created";
      auto column = all_cf_names[cf_name];
      handles_[column].reset(new_handle);
    } else {
      CHECK(false) << "Failed to create Column Family " << cf_name
                   << ", status=" << status.ToString();
    }
  }

  for (int i = 0; i < existing_cf_handles.size(); ++i) {
    auto column = all_cf_names[existing_cf_names[i]];
    handles_[column].reset(existing_cf_handles[i]);
  }

  for (const auto& iter : column_options) {
    auto column_index = iter.first;
    const auto& column_option = iter.second;
    auto handle = handles_[column_index];
    std::unordered_map<std::string, std::string> option_string;
    ConfigColumnOptionString(column_option, &option_string);
    auto status = db_->SetOptions(handle.get(), option_string);
    CHECK(status.ok()) << "Failed to set column option";
    LOG(INFO) << "Set column option " << handle->GetName();
    for (const auto& iter : option_string) {
      LOG(INFO) << iter.first << ": " << iter.second;
    }
  }
}

void LocalDB::CloseDB() {
  db_->Flush(rocksdb::FlushOptions());
  handles_.clear();
  delete db_;
}

std::shared_ptr<rocksdb::ColumnFamilyHandle> LocalDB::GetColumnHandle(
    LocalDB::Column column) {
  return handles_[column];
}

void LocalDB::ClearColumn(Column column) {
  auto prev_handle = handles_[column];
  auto column_name = prev_handle->GetName();
  db_->DropColumnFamily(prev_handle.get());

  rocksdb::ColumnFamilyHandle* new_handle = nullptr;
  rocksdb::ColumnFamilyDescriptor cf;
  cf.name = column_name;
  auto status = db_->CreateColumnFamily(rocksdb::ColumnFamilyOptions(),
                                        column_name, &new_handle);
  CHECK(status.ok()) << "Faield to recreat column " << column_name
                     << ", status=" << status.ToString();
  handles_[column].reset(new_handle);
}

rocksdb::Status LocalDB::Write(const std::string& key,
                               const std::string& value) {
  return Write(key, value, LocalDB::COLUMN_DEFAULT);
}

rocksdb::Status LocalDB::Write(const rocksdb::Slice& key,
                               const rocksdb::Slice& value, Column column) {
  const int WRITE_RETRY_COUNT = 2;
  int retry_count = 0;
  rocksdb::Status status;
  while (retry_count <= WRITE_RETRY_COUNT) {
    status =
        db_->Put(rocksdb::WriteOptions(), handles_[column].get(), key, value);
    if (status.ok()) {
      break;
    } else {
      std::this_thread::yield();
      ++retry_count;
    }
  }
  CHECK(status.ok()) << "FATAL ERROR. Failed to write DB with retry. status="
                     << status.ToString();
  return status;
}

rocksdb::Status LocalDB::Read(const rocksdb::Slice& key, std::string* value) {
  return Read(key, value, LocalDB::COLUMN_DEFAULT);
}

rocksdb::Status LocalDB::Read(const rocksdb::Slice& key, std::string* value,
                              Column column) {
  rocksdb::ReadOptions option;
  return db_->Get(option, handles_[column].get(), key, value);
}

std::unique_ptr<rocksdb::Iterator> LocalDB::GetIterator(Column column) {
  return std::unique_ptr<rocksdb::Iterator>(
      db_->NewIterator(rocksdb::ReadOptions(), handles_[column].get()));
}

std::unique_ptr<rocksdb::Iterator> LocalDB::GetLast(Column column) {
  auto iterator =
      db_->NewIterator(rocksdb::ReadOptions(), handles_[column].get());
  iterator->SeekToLast();
  return std::unique_ptr<rocksdb::Iterator>(iterator);
}

}  // namespace cache
}  // namespace zhi
