#include "utility.hpp"
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
constexpr auto GetTopTen = GetTopN<T, 10>;
// https://stackoverflow.com/questions/44521991/type-trait-to-get-element-type-of-stdarray-or-c-style-array
template <typename T>
using element_type_t =
    std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;
template <typename... Args>
bool logicalAnd(Args... args) {
    // Binary folding.
    return (true && ... && args);
}
template <typename... Args>
auto sum(Args... args) {
    // Unary folding.
    return (... + args);
}
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
template <typename Callable>
class Proxy {
    Callable c;

public:
    Proxy(Callable c) : c(c) {
    }
    template <class... Args>
    decltype(auto) operator()(Args&&... args) {
        return std::invoke(c, std::forward<Args>(args)...);
    }
};
int TestMacro() {
#define InnerMacro(a, b, c) printf("%s\t%s\t%s\n", #a, #b, #c);
#define Macro(id)                                                              \
    do {                                                                       \
        id                                                                     \
    } while (0)
    Macro(InnerMacro(1, 3, 3));
    return 0;
}
int main(int argc, char* argv[]) {
    test_variant();
    std::array<int, 3> click = {3, 20, 4};
    GetTopThree<element_type_t<decltype(click)>>(click, 2);
    std::tuple<int, std::tuple<int>> a;
    fmt::print("is tuple {} another {}\n",
               utils::is_tuple<decltype(click)>::value,
               utils::is_tuple<decltype(a)>::value);

    static_assert(
        std::is_same<std::conditional<true, int, double>::type, int>::value);
    // Binary literals
    auto binary_literal_value = 0b1111'1111; // == 255
    // Generic lambda expressions
    auto identity = [](auto x) { return x; };
    auto three = identity(3);   // == 3
    auto foo = identity("foo"); // == "foo"

    // raw string literal
    const char* msg = R"(Hello,
            world!
    )";
    char16_t    utf8_str[] = u"\u0123";
    fmt::print("{} {} {} \nraw-msg {}\n{}\n",
               binary_literal_value,
               three,
               foo,
               msg,
               msg);
    auto                 add = [](int x, int y) { return x + y; };
    Proxy<decltype(add)> p{add};
    fmt::print("result {} result {}\n",
               p(1, 2),
               std::apply(add, std::make_tuple(1, 2)));
}
