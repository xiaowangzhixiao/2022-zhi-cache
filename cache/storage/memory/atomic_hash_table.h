//
// Created by wangzhi on 18-11-18.
// update on 22-07-22

#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <vector>

namespace zhi {
namespace cache {

template <typename Key, typename Value>
struct HashNode {
  Key key;
  Value value;
  std::shared_ptr<HashNode<Key, Value>> next;
};

template <typename Key, typename Value>
class HashTable {
  using pHashNode = std::shared_ptr<HashNode<Key, Value>>;

 public:
  explicit HashTable(int hint);
  ~HashTable() = default;
  bool Add(Key &&, Value &&);
  Value *Get(const Key &);
  bool Del(const Key &);
  int size() { return _size; }

 private:
  std::vector<std::atomic<pHashNode>> _bucket;
  size_t _bucketSize;
  std::atomic_int _size;
};

template <typename Key, typename Value>
HashTable<Key, Value>::HashTable(int hint) {
  static size_t primes[] = {
      17,         /* 0 */
      37,         /* 1 */
      79,         /* 2 */
      163,        /* 3 */
      331,        /* 4 */
      673,        /* 5 */
      1361,       /* 6 */
      2729,       /* 7 */
      5471,       /* 8 */
      10949,      /* 9 */
      21911,      /* 10 */
      43853,      /* 11 */
      87719,      /* 12 */
      175447,     /* 13 */
      350899,     /* 14 */
      701819,     /* 15 */
      1403641,    /* 16 */
      2807303,    /* 17 */
      5614657,    /* 18 */
      11229331,   /* 19 */
      22458671,   /* 20 */
      44917381,   /* 21 */
      89834777,   /* 22 */
      179669557,  /* 23 */
      359339171,  /* 24 */
      718678369,  /* 25 */
      1437356741, /* 26 */
      2147483647  /* 27 (largest signed int prime) */
  };
  int i;
  for (i = 1; primes[i] < hint; ++i)
    ;

  _bucketSize = primes[i - 1];
  _size = 0;
  _bucket.resize(_bucketSize);
  for (int j = 0; j < _bucketSize; ++j) {
    _bucket[j] = nullptr;
  }
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::Add(Key &&key, Value &&value) {
  pHashNode p;
  unsigned int index;
  index = std::hash<Key>{}(key) % _bucketSize;
  for (p = _bucket[index].load(std::memory_order_relaxed); p; p = p->next) {
    if (key == p->key) {
      break;
    }
  }

  if (p == nullptr) {
    p = std::make_shared<HashNode<Key, Value>>();
    if (p == nullptr) {
      return false;
    }
    p->key = key;
    p->next = bucket[index].load(std::memory_order_relaxed);
    while (!_bucket[index].compare_exchange_weak(
        p->next, p, std::memory_order_release, std::memory_order_relaxed))
      ;  // 循环体为空
    _size++;
  }

  p->value = value;
  return true;
}

template <typename Key, typename Value>
Value *HashTable<Key, Value>::Get(const Key &key) {
  unsigned int index;
  pHashNode p;

  index = std::hash<Key>{}(key) % _bucketSize;
  for (p = _bucket[index].load(std::memory_order_relaxed); p; p = p->next) {
    if (key == p->key) {
      break;
    }
  }

  if (p == nullptr) {
    return nullptr;
  }
  return &(p->value);
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::Del(const Key &key) {
  return true;
}

template <>
bool HashTable<std::string, std::string>::Del(const std::string &key) {
  pHashNode p;
  unsigned int index;
  index = std::hash<std::string>{}(key) % _bucketSize;
  for (p = _bucket[index].load(std::memory_order_release); p; p = p->next) {
    if (key == p->key) {
      break;
    }
  }

  p->value = "";
  p->key = "";
  return true;
}

}  // namespace cache
}  // namespace zhi
