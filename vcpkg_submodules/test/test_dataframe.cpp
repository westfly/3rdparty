#include <DataFrame/DataFrame.h>
#include <DataFrame/DataFrameFinancialVisitors.h>
#include <DataFrame/DataFrameMLVisitors.h>
#include <DataFrame/DataFrameOperators.h>
#include <DataFrame/DataFrameStatsVisitors.h>
#include <DataFrame/RandGen.h>
#include <fmt/format.h>
#include <iostream>
#include <random>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <scn/scn.h>

int split(const std::string_view         str,
          const std::string_view         delims,
          bool                           empty_field,
          std::vector<std::string_view>* output) {
    for (auto first = str.data(),
              second = str.data(),
              last = first + str.size();
         second != last && first != last;
         first = second + 1) {
        second = std::find_first_of(
            first, last, std::cbegin(delims), std::cend(delims));
        // printf("%p:%lu:%lu\n", first, last - first, second - first);
        if (first != second || empty_field)
            output->emplace_back(first, second - first);
    }
    return output->size();
}
using MyDataFrame = hmdf::StdDataFrame<unsigned long>;
typedef rapidjson::Document Record;
struct Table {
    int Init(const std::string& filename) {
#if 0
        scn::owning_file file{filename.c_str(), "r"};
        if (!file.is_open()) {
            fmt::print("open {} failed\n", filename);
            return -1;
        }
#endif
        std::ifstream                 fin(filename);
        std::string                   line;
        std::vector<std::string_view> field_list;
        while (getline(fin, line)) {
            using namespace std::literals;
            split(line, "|"sv, false, &field_list);
            if (field_list[1].find("dsp") != std::string_view::npos) {
                continue;
            }
            bool big_data_set = false;
            if (big_data_set || field_list[0].find("48bc47c808ad437a3f6e5cf0e2c103ee") !=
                    std::string_view::npos) {
                fmt::print("new field {}\n", field_list[3]);
                std::string id(field_list[0]);
                // json_str_list.emplace(id, field_list[3]);
                json_str_list.emplace_back(std::string(field_list[3]));
                MakeRecord(json_str_list.back());
            }
        }
        fmt::print("load {}:{} record from {} \n",
                   json_str_list.size(),
                   records.size(),
                   filename);
        return 0;
    }
    int MakeRecord(const std::string& sv) {
        rapidjson::Document doc{};
        doc.ParseInsitu(const_cast<char*>(sv.data()));
        if (doc.HasParseError()) {
            fmt::print("Get Error {} and Offset {}\n",
                       doc.GetParseError(),
                       doc.GetErrorOffset());
            return -1;
        }
        records.emplace_back(std::move(doc));
        return 0;
    }
    size_t Size() const {
        return records.size();
    }
    auto& At(size_t idx) const {
        assert(idx <= Size());
        return records[idx];
    }

private:
    // std::map<std::string, std::string> json_str_list;
    std::vector<std::string> json_str_list;
    std::vector<Record>      records;
};
using namespace hmdf;
static const char* kTypeNames[] = {
    "Null", "False", "True", "Object", "Array", "String", "Number"};
template <typename I, typename H>
int TableToDataFrame(const Table& tlb, DataFrame<I, H>& df) {
    const int64_t rows = tlb.Size();
    if (rows <= 0) {
        return -1;
    }
    // DataFrame requires sequence index.
    const int kMaxTableSize = 5;
    df.load_data(DataFrame<I, H>::gen_sequence_index(0, rows));
    for (int i = 0; i < rows; ++i) {
        const auto& document = tlb.At(i);
        for (auto itr = document.MemberBegin(); itr != document.MemberEnd();
             ++itr) {
            auto cpp_value_type = itr->value.GetType();
            auto field_name = itr->name.GetString();
            switch (cpp_value_type) {
#define CASE_BUILD(                                                            \
    cpp_value_type, column_type, df, field_name, field_value, Fn)              \
    case cpp_value_type: {                                                     \
        fmt::print(                                                            \
            "Name:{} TypeName:{}\n", field_name, kTypeNames[cpp_value_type]);  \
        column_type val;                                                       \
        if constexpr (std::is_same<column_type, std::string>::value) {         \
            val = std::string(field_value.GetString(),                         \
                              field_value.GetStringLength());                  \
        }                                                                      \
        else {                                                                 \
            val = field_value.Get##Fn();                                       \
        }                                                                      \
        if (df.has_column(field_name)) {                                       \
            auto& v = df.template get_column<column_type>(field_name);         \
            v.push_back(val);                                                  \
        }                                                                      \
        else {                                                                 \
            auto& v = df.template create_column<column_type>(field_name);      \
            v.push_back(val);                                                  \
        }                                                                      \
        break;                                                                 \
    }
                CASE_BUILD(5, std::string, df, field_name, itr->value, String);
                CASE_BUILD(6, int64_t, df, field_name, itr->value, Int64);
            default: {
                break;
            }
            }
        }
    }
    std::default_random_engine          e;
    std::uniform_int_distribution<long> u(-30000, 999999);
    df.template retype_column<std::string, long>(
        "appid", [&e, &u](const std::string& val) {
            fmt::print("gooogle {}\n", val);
            return (std::stoull(val) + u(e));
        });
    auto&            column_value = df.template get_column<long>("appid");
    SumVisitor<long> sum_visitor;
    auto sum = df.template visit<long>("appid", sum_visitor).get_result();
    NLargestVisitor<3, long> nl_visitor;
    df.template visit<long>("appid", nl_visitor);
    std::string nlargest_str;
    for (auto& iter : nl_visitor.get_result()) {
        nlargest_str.append(fmt::format("{}|{},", iter.index, iter.value));
    }
    NSmallestVisitor<3, long> ns_visitor;
    df.template visit<long>("appid", ns_visitor);
    std::string nsmall_str;
    for (auto& iter : ns_visitor.get_result()) {
        nsmall_str.append(fmt::format("{}|{},", iter.index, iter.value));
    }
    fmt::print("shape {}:{} column_value {} {} and "
               "sum()={}\nnlargets={}\nsmallest={}\n",
               df.shape().first,
               df.shape().second,
               column_value.size(),
               column_value[0],
               sum,
               nlargest_str,
               nsmall_str);
    // multi-visit
    // AbsVisitor<long> abs_visistor;
    MeanVisitor<long> abs_visitor;
    df.template multi_visit(std::make_pair("appid", &nl_visitor),
                            std::make_pair("appid", &ns_visitor),
                            std::make_pair("appid", &abs_visitor),
                            std::make_pair("appid", &sum_visitor));
    assert(sum == sum_visitor.get_result());
    StepRollAdopter<MeanVisitor<long>, long> roller_visitor(MeanVisitor<long>(),
                                                            1);
    ReturnVisitor<long> return_visit(return_policy::monetary);
    df.template sort<long>("time", sort_spec::ascen);
    df.template single_act_visit<long>("time", return_visit);
    df.template load_column<long>("time_diff", return_visit.get_result());
    df.template write<std::ostream, int, long, double, std::string>(std::cout);
#if 0
    // groupby
    auto ndf = df.template groupby1<std::string>(
        "isexp",
        LastVisitor<MyDataFrame::IndexType, MyDataFrame::IndexType>(),
        std::make_tuple("appid", "max", MaxVisitor<long>()),
        std::make_tuple("appid", "sum", SumVisitor<long>()),
        std::make_tuple("appid", "min", MinVisitor<long>()));
    ndf.template write<std::ostream, int, long, double, std::string>(std::cout);
#endif
    return 0;
}
int main(int argc, char* argv[]) {
    Table table;
    table.Init("../data/cassandra_ups_event.log");
    MyDataFrame df;
    TableToDataFrame(table, df);
    return 0;
}