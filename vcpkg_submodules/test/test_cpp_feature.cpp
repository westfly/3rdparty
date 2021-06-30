#include <algorithm>
#include <array>
#include <cassert>
#include <dbg.h>
#include <fmt/format.h>
#include <string>
#include <variant>
template <typename T, size_t N>
void GetTopN(std::array<T, N>& array, size_t n) {
    auto        len = std::min(N, n);
    std::string debug;
    for (auto i = 0; i < len; ++i) {
        debug.append(fmt::format("{} {},", i, array[i]));
    }
    if (debug.size()) {
        debug.pop_back();
    }
    fmt::print("{}\n", debug);
}
template <typename T>
constexpr auto GetTopThree = GetTopN<T, 3>;
template <typename T>
constexpr auto GetTopTen= GetTopN<T, 10>;
// https://stackoverflow.com/questions/44521991/type-trait-to-get-element-type-of-stdarray-or-c-style-array
template <typename T>
using element_type_t =
    std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

void test_variant() {
    std::variant<int, float> w;
    w = 12;
    try {
        fmt::print("{}\n", std::get<int>(w));
    }
    catch (const std::bad_variant_access&) {
        fmt::print("error\n");
    }
}

int main(int argc, char* argv[]) {
    test_variant();
    std::array<int, 3> click = {3, 20, 4};
    GetTopThree<element_type_t<decltype(click)>>(click, 2);
}
