#include <dbg.h>
#include <variant>
#include <string>
#include <cassert>

void test_variant() {
    std::variant<int, float> w;
    w = 12;
    try {
      std::get<float>(w);
    }
    catch (const std::bad_variant_access&) {}
}

int main(int argc, char* argv[]) {

}