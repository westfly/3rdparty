#include <bits/stdc++.h>
#include <boost/preprocessor/seq/for_each.hpp>
#define seq (a)(b)(c)(d)
#define func(r, data, elem) printf("%d %d %d\n", r, data, elem);

#define seq_ (a, 2)(b, 1008)(c, 100) // this is not correct, using STL instead.

#define MAP_KEY_TYPES (int)(double)(std::string)
#define MAP_VAL_TYPES (int)(double)(std::string)(std::vector<int>)

#define DECLARE_MAP_VARIANTS(r, K, V)                                          \
    void toJson(Json::Value& j, const std::map<K, V>& v);                      \
    void fromJson(const Json::Value& j, std::map<K, V>& v);

#define DECLARE_MAP_VARIANTS_KEY(r, data, K)                                   \
    BOOST_PP_SEQ_FOR_EACH(DECLARE_MAP_VARIANTS, K, MAP_VAL_TYPES)

#define DECLARE_MAP_VARIANTS_(r, KV)                                           \
    DECLARE_MAP_VARIANTS(r, BOOST_PP_SEQ_ELEM(0, KV), BOOST_PP_SEQ_ELEM(1, KV))

int main() {
    int a = 1, b = 2, c = 3, d = 4;
    BOOST_PP_SEQ_FOR_EACH(DECLARE_MAP_VARIANTS_KEY, _, MAP_KEY_TYPES)
    BOOST_PP_SEQ_FOR_EACH_PRODUCT(DECLARE_MAP_VARIANTS_,
                                  (MAP_KEY_TYPES)(MAP_VAL_TYPES))
    /*
     * expand to:
     * printf("%d %d %d\n", r, data, a);
     * printf("%d %d %d\n", r, data, b);
     * printf("%d %d %d\n", r, data, c);
     * for every element in macro seq will replace the elem in func.
     */
    return 0;
}