#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <absl/strings/string_view.h>
#include <dbg.h>
#include <fmt/format.h>
#include <iostream>
#include <string>
// https://stackoverflow.com/questions/4546021/remove-char-from-stringstream-and-append-some-data
std::string Format(std::vector<int>& data) {
    std::ostringstream douCoh;
    douCoh << '{';
    for (unsigned int i = 0; i < data.size(); i++)
        douCoh << i + 1 << ':' << data[i] << ',';
    douCoh.seekp(-1, douCoh.cur);
    douCoh << '}';
    return douCoh.str();
}
int main() {
    std::string       s = "Tired like a dog";
    absl::string_view sv(s); // 用 std::string 初始化
    fmt::print("s={} sv={} \n", s, sv);
    std::string str = " world";
    const char* cstr = " abc ";
    int         i = 55;
    double      f = 123.44678;
    std::string res = absl::StrCat(sv, str, cstr, i, f);
    // absl::PrintF("absl::PrintF = %s", sv);
    absl::StrAppend(&res, "\nhello 0x", absl::Hex(i, absl::kZeroPad4));
    fmt::print("res={}\n", res);
    std::vector<int> v1 = {1,2,3,33,3};
    fmt::print("ostreamstream = {}\n", Format(v1));
}