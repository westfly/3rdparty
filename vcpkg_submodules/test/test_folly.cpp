#include <folly/CpuId.h>
#include <folly/concurrency/ConcurrentHashMap.h>
#include <folly/portability/GTest.h>
#include <folly/Conv.h>
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
TEST(conv, String) {
    fbstring str;
    toAppend(2.5, &str);
    CHECK_EQ(str, "2.5");
    toAppend(" is ", 2, " point ", 5, &str);
    CHECK_EQ(str, "2.5 is 2 point 5");

    auto a = to<fbstring>(123);
    CHECK_EQ(a, "123");
    a = to<fbstring>(-456);
    CHECK_EQ(a, "-456");
}

static int NextPowerOfTwo32(int aNumber)
{
#if defined(__arm__)
    return 1 << (32 - __builtin_clz(aNumber - 1));
#else
    --aNumber;
    aNumber |= aNumber >> 1;
    aNumber |= aNumber >> 2;
    aNumber |= aNumber >> 4;
    aNumber |= aNumber >> 8;
    aNumber |= aNumber >> 16;
    return ++aNumber;
#endif
}

TEST(Algo, NextPower32) {
    CHECK_EQ(NextPowerOfTwo32(30), 32);
    CHECK_EQ(NextPowerOfTwo32(1), 1);
}