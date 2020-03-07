#include <iostream>
#ifdef SINGLE_HEADER
#include "cista.h"
#else
#include "cista/containers/string.h"
#endif

using cista::raw::string;

constexpr auto const CORNER_CASE_SHORT_14 = "01234567891234";
constexpr auto const CORNER_CASE_SHORT_15 = "012345678912345";
constexpr auto const CORNER_CASE_LONG_16 = "0123456789123456";
constexpr auto const LONG_STR = "hello world hello world";
constexpr auto const SHORT_STR = "hello world";
int main(int argc, const char *argv[])
{
    auto s0 = string{LONG_STR, string::owning};
    auto s1 = string{s0};
    printf("%d\n", s1 == s0);
    return 0;
}
