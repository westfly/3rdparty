#include

namespace utils {

// https://stackoverflow.com/questions/13101061/detect-if-a-type-is-a-stdtuple
/*
if constexpr (is_tuple<Type>::value) {
}
*/
template <typename>
struct is_tuple : std::false_type {};

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};
}