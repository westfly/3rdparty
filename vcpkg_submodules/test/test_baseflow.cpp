#include "boost/pfr.hpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "enumerate.hpp"
#include "magic_enum.hpp"
//#include "pprint/pprint.hpp"
#define DBG_MACRO_NO_WARNING 1
#include "dbg.h"
#include "fast_float/fast_float.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "static_reflection.h"
#include "struct.h"
#include "zip.hpp"
#include <list>

TEST_CASE("baseflow", "[spdlog]") {
    auto file_logger = spdlog::rotating_logger_mt<spdlog::async_factory>(
        "ml_realflow", "log.log", 3 * 1024 * 1024, 5);
    spdlog::set_pattern("[%m%d-%H:%M:%S:%e][%l][T%t] %v");
    spdlog::flush_every(std::chrono::seconds(3));
}
TEST_CASE("baseflow_magic", "[magic_enum]") {
    enum class Color { RED = -12, GREEN = 7, BLUE = 15 };
    constexpr auto cr = magic_enum::enum_cast<Color>("red");
    // REQUIRE(cr.value() == Color::RED);
    REQUIRE(magic_enum::enum_cast<Color&>("GREEN").value() == Color::GREEN);
}
struct simple {
    int   a;
    char  b;
    short d;
};
BOOST_PFR_FUNCTIONS_FOR(simple);
TEST_CASE("baseflow_boost", "[boost_get]") {
    simple            x{42, 'a', 3};
    std::stringstream ss;
    boost::pfr::for_each_field(x, [&ss](auto&& val) { ss << val << '\t'; });
    dbg(ss.str());
    dbg(x);
}
TEST_CASE("baseflow_cpp17", "[cpp17]") {
    dbg("yang");
}
/*
TEST_CASE("baseflow_pprint", "[pprint]") {
    pprint::PrettyPrinter printer;
    printer.print("Hello, 世界");
    printer.print(nullptr);
}
*/
TEST_CASE("baseflow_reflection", "[static_reflection]") {
    SimpleStruct simple{false, 10, 1.1, "Rayan", false};
    ForEachField(simple, [](auto&& field_value, auto&& field_name) {
        dbg(fmt::format("{} {}", field_name, field_value));
    });
}
TEST_CASE("baseflow_enumerate", "[enumerate]") {
    std::list<std::string> lst{"hello", "world!"};
    for (const auto& [i, v] : utils::enumerate(lst)) {
        dbg(fmt::format("{} {}", i, v));
    }
}
TEST_CASE("baseflow_zip", "[zip]") {
    // type must have operation +=
    std::vector<int>         id_list{11, 12, 13};
    std::vector<std::string> name_list{"John", "Rayan"};
    for (const auto& [id, name] : utils::zip(id_list, name_list)) {
        dbg(fmt::format("{} {}", id, name));
    }
    auto        a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto        b = std::vector<int>{1, 2, 3, 4, 5, 6, 7};
    auto        c = std::vector<int>{0, 0, 0, 0, 0};
    auto const& d = b;

    for (auto&& [x, y, z] : utils::zip(a, d, c)) {
        z = x + y;
    }
    dbg(c);
}

TEST_CASE("baseflow_fastfloat", "[float]") {
    const std::string input = "3.1416 xyz ";
    double            result;
    auto              answer = fast_float::from_chars(
        input.data(), input.data() + input.size(), result);
    dbg(fmt::format("ans={} {} {}", result, answer.ptr, answer.ec));
}
