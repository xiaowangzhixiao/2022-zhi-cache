#pragma once

#include <string>

#include "cache/model/request_model.h"
#include "cache/storage/memory/concurrent_hash_map.h"
#include "cache/storage/memory/skip_list.h"
#include "cache/utils/arena.h"

namespace zhi {
namespace cache {

using ZsetSkipListNode = ZaddRequest;

struct Comparator {
  int operator()(const ZsetSkipListNode& a, const ZsetSkipListNode& b) const {
    if (a.score < b.score) {
      return -1;
    } else if (a.score > b.score) {
      return +1;
    } else {
      return 0;
    }
  }
};

class Zset {
 public:
  Zset(std::shared_ptr<Arena> arena)
      : _arena(arena), _score_range_map(comparator, _arena.get()) {
    _value_map.Init(87719);
  };
  ~Zset() = default;

  bool Add(int32_t score, std::string& value) {
    std::string copy_value(value);
    int32_t copy_score = score;
    _value_map.Add(std::move(copy_value), std::move(copy_score));
    ZsetSkipListNode zsetSkipListNode;
    zsetSkipListNode.score = score;
    zsetSkipListNode.value = std::move(value);
    _score_range_map.Insert(zsetSkipListNode);
    return true;
  }

  std::vector<ZsetSkipListNode> Range(int32_t min_score, int32_t max_score) {
    SkipList<ZsetSkipListNode, Comparator>::Iterator iter(&_score_range_map);
    ZsetSkipListNode min_node;
    min_node.score = min_score;
    iter.Seek(min_node);
    std::vector<ZaddRequest> result;
    while (iter.Valid() && iter.key().score <= max_score) {
      auto* score = _value_map.Get(iter.key().value);
      if (score != nullptr && *score == iter.key().score) {
        ZsetSkipListNode node;
        node.score = iter.key().score;
        node.value = iter.key().value;
        result.push_back(std::move(node));
      }
      iter.Next();
    }
    return result;
  }

  bool Del(const std::string& value) { return _value_map.Del(value); }

 private:
  Comparator comparator;
  std::shared_ptr<Arena> _arena;
  HashKv<int32_t> _value_map;
  SkipList<ZsetSkipListNode, Comparator> _score_range_map;
};

}  // namespace cache

}  // namespace zhi
