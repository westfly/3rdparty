#include <iostream>
#include <rapidcsv.h>
#include <string>
#define DBG_MACRO_NO_WARNING 1
#include <dbg.h>
#include <fmt/format.h>
std::string kJson = R"({
    "hello": "world",
    "t": true ,
    "f": false,
    "n": null,
    "i": 123,
    "pi": 3.1416,
    "a": [1, 2, 3, 4]
})";
#define RAPIDJSON_HAS_STDSTRINGVIEW 1
#include "rapidjson/document.h"

using namespace rapidjson;

int TestJsonOnline() {
    std::cout << kJson;
    Document document;
    document.ParseInsitu(const_cast<char*>(kJson.data()));
    const char* str = kJson.data();
    using namespace std::literals;
    auto key = "hello";
    assert(document.HasMember(key));
    assert(document[key].IsString());
    auto sv = document[key].GetString();
    printf("\n%s = %s\t %p %p %ld\n", key, sv, sv, str, sv - str);
    printf("\njson=%s", kJson.c_str());

    auto iter = document.FindMember(key);
    if (iter != document.MemberEnd()) {
        printf("\n%s\t === %d\n",
               iter->value.GetString(),
               iter->value.GetStringLength());
    }
    return 0;
}
/*
https://stackoverflow.com/questions/50051473/if-constexpr-in-c17-does-not-work-in-a-non-templated-function
constexpr only work in template, fake one
*/
template <typename T>
int ConvertToDataFrame(const rapidjson::Document& d) {
    for (auto itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr) {
        auto cpp_value_type = itr->value.GetType();
        auto field_name = itr->name.GetString();
        fmt::print("ConvertToDataFrame cpp_value_type = {}, field_name = {}\n",
                   cpp_value_type,
                   field_name);
        switch (cpp_value_type) {
#define CASE_BUILD(                                                            \
    cpp_value_type, column_type, df, field_name, field_value, Fn)              \
    case cpp_value_type: {                                                     \
        column_type val;                                                       \
        if constexpr (std::is_same<column_type, std::string>::value) {         \
            val = std::string(field_value.GetString(),                         \
                              field_value.GetStringLength());                  \
        }                                                                      \
        else {                                                                 \
            val = field_value.Get##Fn();                                       \
        }                                                                      \
        break;                                                                 \
    }
        CASE_BUILD(
                5, std::string, field_name, field_name, itr->value, String);
        CASE_BUILD(6, int64_t, field_name, field_name, itr->value, Int64);
        default: {
            break;
        }
        }
    }
    return 0;
}
int ReadDocumentFromFile(const std::string& filename) {
    std::vector<rapidjson::Document> doc_list;
    rapidcsv::Document               doc(
        filename,
        rapidcsv::LabelParams(0, -1),
        rapidcsv::SeparatorParams('\t' /* pSeparator */,
                                  false /* pTrim */,
                                  rapidcsv::sPlatformHasCR /* pHasCR */,
                                  false /* pQuotedLinebreaks */,
                                  true /* pAutoQuote */));

    fmt::print("{} {} {}\n",
               doc.GetRowCount(),
               doc.GetColumnCount(),
               doc.GetColumnCount());
    dbg(doc.GetColumnNames());
    auto rows = doc.GetRowCount();
    doc_list.reserve(rows);
    for (auto i = 0; i < rows; ++i) {
        auto row = doc.GetRow<std::string>(i);
        if (i % 50 == 0) {
            dbg(fmt::format("{} {}", row[1], row[3]));
        }
        rapidjson::Document d{};
        // if (d.ParseInsitu(const_cast<char*>(row[3].data())).HasParseError())
        // {
        if (d.Parse(row[3].data()).HasParseError()) {
            fmt::print("Get Error {} and Offset {}\n",
                       d.GetParseError(),
                       d.GetErrorOffset());
            continue;
        }
        doc_list.emplace_back(std::move(d));
    }
    fmt::print("get doc_size {}\n", doc_list.size());
    auto        random_idx = 99;
    const auto& d = doc_list[random_idx];
    ConvertToDataFrame<int>(d);
    return 0;
}
int main(int argc, char const* argv[]) {
    ReadDocumentFromFile("../data/test_data_total.csv");
    return 0;
}
