#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <absl/strings/string_view.h>
#include <algorithm>
#include <dbg.h>
#include <fmt/format.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <absl/hash/hash.h>
#include "struct.h"

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
const float kUnValidScore = 1.5f;

int get_top_k(int k, float max_score) {
    std::vector<Feature>             topk;
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> real_dis(1.0, 2.0);
    std::uniform_int_distribution<>  int_dis(1, 6);
    int                              MaxLength = 2 * k;
    std::vector<Feature>             strange_list;
    for (int i = 0; i < k; ++i) {
        auto ele = Feature{int_dis(gen), real_dis(gen)};
        if (ele.score >= max_score) {
            continue;
        }
        topk.emplace_back(std::move(ele));
    }
    for (auto iter = topk.begin(); iter != topk.end(); ++iter) {
        dbg(*iter);
    }
    auto cmp_fn = std::greater<Feature>();
    std::make_heap(topk.begin(), topk.end(), cmp_fn);
    for (int i = k; i < MaxLength; ++i) {
        auto score = real_dis(gen);
        auto ele = Feature{int_dis(gen), score};
        // dbg(fmt::format("{}\t{}", topk.front().score, score));
        if (score >= max_score) {
            strange_list.emplace_back(std::move(ele));
            continue;
        }
        if ((topk.front().score <= ele.score)) {
            dbg(fmt::format("{} less\t{}", topk.front().score, score));
            std::pop_heap(topk.begin(), topk.end(), cmp_fn);
            topk.pop_back();
            topk.emplace_back(std::move(ele));
            std::push_heap(topk.begin(), topk.end(), cmp_fn);
        }
        else {
            dbg(fmt::format("greater {} {}", topk.front().score, score));
        }
    }
    std::sort_heap(topk.begin(), topk.end(), cmp_fn);
    int i = 0;
    while (topk.size() < k) {
        topk.push_back(std::move(strange_list[i]));
        ++i;
    }
    for (auto iter = topk.begin(); iter != topk.end(); ++iter) {
        dbg(*iter);
    }
    return 0;
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
    std::vector<int> v1 = {1, 2, 3, 33, 3};
    fmt::print("ostreamstream = {}\n", Format(v1));
    fmt::print("{} {} \n",
               std::tuple_size<std::tuple<std::string>>::value,
               std::tuple_size<std::tuple<void>>::value);
    //get_top_k(10, kUnValidScore);
    return 0;
}