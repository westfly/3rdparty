#include "fmt/color.h"
#include "fmt/format.h"
#include "fmt/printf.h"
int main(int argc, const char *argv[])
{
    fmt::printf("Hello, %s!\n", "world");
    fmt::memory_buffer out;
    std::string out_str("score");
    for(int i = 0; i < 10; ++i) {
        fmt::format_to(std::back_inserter(out_str), "The answer is {}.\n", i);
        fmt::format_to(out, "The answer is {}.\n", i);
    }
    fmt::print("addd_string {}\n", out_str);
    fmt::print("addd {}\n", fmt::to_string(out));
    return 0;
}


