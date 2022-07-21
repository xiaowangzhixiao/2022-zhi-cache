#include "cache/manager/kv_manager.h"

#include <string>

#include "gtest/gtest.h"

namespace zhi {
namespace cache {
  TEST(KvManager, All) {
    KvManager* kv_manager = KvManager::Instance();
    EXPECT_TRUE(kv_manager->Init());
    EXPECT_TRUE(kv_manager->Add("wangzhi", "123321"));
    EXPECT_TRUE(kv_manager->Add("limanting", "321123"));
    EXPECT_EQ("123321", *(kv_manager->Query("wangzhi")));
    EXPECT_TRUE(kv_manager->Del("wangzhi"));
    EXPECT_EQ(nullptr, kv_manager->Query("wangzhi"));
  }
}  // namespace cache
}  // namespace zhi
