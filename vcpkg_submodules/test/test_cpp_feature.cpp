#include <dbg.h>
#include <variant>
#include <string>
#include <cassert>
#include <fmt/format.h>

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
}
