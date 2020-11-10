#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <stdio.h>
#include <string>
int sum(const std::string& tag, int a, int b, int c) {
    return a + b + c;
}
#define MacroInternal(r, data, elem, fn, xobj, xa, xb, xc)                     \
    do {                                                                       \
        xobj = fn(BOOST_PP_STRINGIZE(elem), xa, xb, xc);                       \
        printf("%s\t %d\n", BOOST_PP_STRINGIZE(elem), xobj);                   \
    } while (0)

#define MacroHelper(r, data, elem)                                             \
    MacroInternal(r, data, elem, sum, xobj, xa, xb, xc);

#define SumResult(obj, a, b, c, macro_seq)                                     \
    do {                                                                       \
        auto xobj = obj, xa = a, xb = b, xc = c;                               \
        BOOST_PP_SEQ_FOR_EACH(MacroHelper, _, macro_seq);                      \
    } while (0)

int main() {
// clang-format off
#define CLK_EVENT_SEQ  (adtype_clk)             \
                       (adtype_pkg_clk)         \
                       (adtype_appid_clk)       \
                       (adtype_appid_pkg_clk)   \
                       (total_clk)              \
                       (total_pkg_clk)
    // clang-format on
    int a = 1, b = 3;
    int c = 5;
    int obj = 0;
    SumResult(obj, a, b, c, CLK_EVENT_SEQ);
    a = 100;
    SumResult(obj, a, b, c, CLK_EVENT_SEQ);
    b = 100;
    SumResult(obj, 1, 2, 3, CLK_EVENT_SEQ);
    c = 200;
    SumResult(obj, 1, 2, 3, CLK_EVENT_SEQ);
    return 0;
}