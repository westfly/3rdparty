#include <gflags/gflags.h>
#include <fmt/format.h>
DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_uint32(port, 8080, "listening port");
DEFINE_string(languages,
              "english,french,german",
              "comma-separated list of languages to offer in the 'lang' menu");
int main(int argc, char * argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    fmt::print("{} {} {}\n", FLAGS_big_menu, FLAGS_port, FLAGS_languages);
    const std::string kConfigStr="--big_menu=true\n--port=89999\n";
    gflags::ReadFlagsFromString(kConfigStr, argv[0], false);
    fmt::print("{} {} {}\n", FLAGS_big_menu, FLAGS_port, FLAGS_languages);
    const std::string kConfigStr2="--big_menu=false\n";
    gflags::ReadFlagsFromString(kConfigStr2, argv[0], false);
    fmt::print("{} {} {}\n", FLAGS_big_menu, FLAGS_port, FLAGS_languages);
    return 0;
}
