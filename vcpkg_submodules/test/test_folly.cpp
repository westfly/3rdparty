#include <folly/CpuId.h>

#include <folly/portability/GTest.h>

using namespace folly;

TEST(CpuId, Simple) {
  CpuId id;
  // All x64 CPUs should support MMX
  EXPECT_EQ(kIsArchAmd64, id.mmx());
}