#include "cache/manager/zset_manager.h"

#include <string>

#include "gtest/gtest.h"

namespace zhi {
namespace cache {
TEST(ZsetManager, All) {
  ZsetManager* zset_manager = ZsetManager::Instance();
  EXPECT_TRUE(zset_manager->Init());
  EXPECT_TRUE(zset_manager->Add("yuwen", 1, "wangzhi"));
  EXPECT_TRUE(zset_manager->Add("yuwen", 5, "limanting"));

  auto range_res = zset_manager->Range("yuwen", 0, 10);
  EXPECT_TRUE(range_res.first);
  EXPECT_EQ(range_res.second.size(), 2);
  EXPECT_EQ(1, range_res.second[0].score);
  EXPECT_EQ("wangzhi", range_res.second[0].value);
  EXPECT_EQ(5, range_res.second[1].score);
  EXPECT_EQ("limanting", range_res.second[1].value);

  EXPECT_TRUE(zset_manager->Del("yuwen", "wangzhi"));
  range_res = zset_manager->Range("yuwen", 0, 10);
  EXPECT_TRUE(range_res.first);
  EXPECT_EQ(range_res.second.size(), 1);
  EXPECT_EQ(5, range_res.second[0].score);
  EXPECT_EQ("limanting", range_res.second[0].value);
}
}  // namespace cache
}  // namespace zhi
