#include "cache/manager/localdb_manager.h"

#include <string>

#include "cache/manager/kv_manager.h"
#include "gtest/gtest.h"

namespace zhi {
namespace cache {

TEST(LocaldbManager, testAll) {
  LocaldbManager* localdb_manager = LocaldbManager::Instance();
  EXPECT_TRUE(localdb_manager->Init("data"));
  EXPECT_TRUE(localdb_manager->Query());

  EXPECT_EQ(
      *KvManager::Instance()->Query("f7ad3b1b-4959-4b6f-9364-78fce33f7f34"),
      std::string("hello world"));
}

}  // namespace cache
}  // namespace zhi
