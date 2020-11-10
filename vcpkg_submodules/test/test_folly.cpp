#include <folly/CpuId.h>
#include <folly/concurrency/ConcurrentHashMap.h>
#include <folly/portability/GTest.h>
#include <fmt/printf.h>
using namespace folly;

TEST(CpuId, Simple) {
    CpuId id;
    // All x64 CPUs should support MMX
    EXPECT_EQ(kIsArchAmd64, id.mmx());
}

TEST(hashmap, Container) {
    folly::ConcurrentHashMap<std::string, int> hashmap;
    hashmap.insert("Tom", 0);
    hashmap.insert("Lilly", 1);
    for (const auto& iter : hashmap) {
        fmt::print("{} {}\n", iter.first, iter.second);
    }
}