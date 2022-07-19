#pragma once

#include <string>

namespace zhi {
namespace cache {
struct LocalDbOption {
  std::string path;
  uint64_t write_buffer_size_in_mb;
  uint64_t max_write_buffer_number;
  uint64_t max_background_compactions;
  uint64_t compaction_readahead_size;
  uint64_t min_write_buffer_number_to_merge;
  int64_t block_cache_size_in_mb;
  uint64_t ttl;

  LocalDbOption() {
    this->write_buffer_size_in_mb = 100;
    this->max_write_buffer_number = 4;
    this->max_background_compactions = 2;
    this->compaction_readahead_size = 0;
    this->min_write_buffer_number_to_merge = 1;
    this->block_cache_size_in_mb = 100;
    this->ttl = -1;
  }
};
}  // namespace cache
}  // namespace zhi
