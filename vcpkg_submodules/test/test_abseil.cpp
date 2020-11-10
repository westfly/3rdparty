#include <absl/strings/str_cat.h>
#include <absl/strings/string_view.h>
#include <absl/strings/str_format.h>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <dbg.h>
int main() {
    std::string       s = "Tired like a dog";
    absl::string_view sv(s); // 用 std::string 初始化
    fmt::print("s={} sv={} \n", s, sv);
    std::string str = " world";
    const char* cstr = " abc ";
    int         i = 55;
    double      f = 123.44678;
    std::string res = absl::StrCat(sv, str, cstr, i, f);
    //absl::PrintF("absl::PrintF = %s", sv);
    absl::StrAppend(&res, "\nhello 0x", absl::Hex(i, absl::kZeroPad4));
    fmt::print("res={}\n", res);
    dbg(res);
    return 0;
}