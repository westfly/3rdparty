#include <cstdio>
#include <scn/scn.h>
#include <spdlog/spdlog.h>

int main() {
    int i;
    // Read an integer from stdin
    // with an accompanying message
    scn::prompt("What's your favorite number? ", "{}", i);
    printf("Oh, cool, %d!", i);

    auto ret = scn::input("{}", i);

    double d;
    ret = scn::input("{}", d);

    long long ll;
    ret = scn::input("{}", ll);

    std::string str;
    ret = scn::input("{}", str);

    int  a, b;
    auto result = scn::scan("0 1 2", "{} {}", a, b);
    // fails, i still uninitialized
    if (!result) {
        spdlog::error("{}", result.error().msg());
    }
}
