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
#include "tuple_helper.h"
#include <DataFrame/RandGen.h>

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
            if (big_data_set ||
                field_list[0].find("48bc47c808ad437a3f6e5cf0e2c103ee") !=
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

template <typename I, typename H>
int extractTopNFea(DataFrame<I, H>& df, std::string type = "Imp") {
    auto df_size = df.get_index().size();
    auto uniq_pkg_vec = df.template get_col_unique_values<std::string>("pkg");
    int  mode_len = (int)uniq_pkg_vec.size();
    std::cout << "in func: extractTopNFea, " << "df_zize=" << df_size << ", mode_len=" << mode_len << std::endl;
    ModeVisitor<3, std::string> mode_visit;
    const auto&                 result =
        df.template single_act_visit<std::string>("pkg", mode_visit)
            .get_result();
    std::cout << "topNFeaof" << type << ":";
    for (auto& i : result) {
        std::cout << i.get_value() << ",";
    }
    if (mode_len == 2) {
        ModeVisitor<3, std::string> mode_visit1;
        const auto&                 result1 =
            df.template single_act_visit<std::string>("pkg", mode_visit1)
                .get_result();
        for (auto& i : result) {
            std::cout << i.get_value() << ",";
        }
    }
    if (mode_len >= 3) {
        ModeVisitor<60, std::string> mode_visit2;
        const auto&                 result2 =
            df.template single_act_visit<std::string>("pkg", mode_visit2)
                .get_result();
        for (auto& i : result) {
            std::cout << i.get_value() << ",";
        }
    }
    std::cout << std::endl;

    return 0;
}

template <typename I, typename H>
void extractTopNFeaByTime(DataFrame<I, H>& df, long req_ts) {
    int  count = 0;
    auto functor = [&count, req_ts](const unsigned long&,
                                    const long& val) -> bool {
        return (req_ts + count++) % 2 == 0;
    };
    auto df_24h =
        df.template get_data_by_sel<long, decltype(functor), long, std::string>(
            "time", functor);
    fmt::print("df_24h size {}\n", df_24h.get_index().size());
    if (df_24h.get_index().size()) {
        df_24h.template write<std::ostream, int, long, double, std::string>(
            std::cout);
        ModeVisitor<2, std::string> mode_visit;
        const auto&                 result =
            df_24h.template single_act_visit<std::string>("pkg", mode_visit)
                .get_result();
        std::cout << "topNFeaIn24h:";
        for (auto& i : result) {
            std::cout << i.get_value() << ",";
        }
        std::cout << std::endl;
    }
}

template <typename I, typename H>
void extractEventNumFea(DataFrame<I, H>& df, long req_ts) {
    // use
    auto time_vec = df.template get_column<long>("time");
    for (auto& i : time_vec) {
        i = (req_ts - i) / (24 * 3600);
    }
    std::string field_name = "num_day";
    df.template load_column<long>(field_name, time_vec);
    auto dvec = df.template get_column<long>("num_day");
    // std::for_each(dvec.begin(), dvec.end(), [](const auto &i){std::cout << i
    // << " "; });

    auto num_df = df.template groupby1<long>(
        "num_day",
        LastVisitor<MyDataFrame::IndexType, MyDataFrame::IndexType>(),
        std::make_tuple("num_day", "event_num_day", CountVisitor<long>()));
    // num_df.template write<std::ostream, long, std::string>(std::cout,
    // io_format::csv2);
    auto event_num_day_vec =
        num_df.template get_column<size_t>("event_num_day");
    for (int i = 0; i < std::min(7, (int)event_num_day_vec.size()); i++) {
        fmt::print("event_num_day_{}={},", i + 1, (int)event_num_day_vec[i]);
    };
}

template <typename I, typename H>
void extractSessionFea(DataFrame<I, H>& df, long req_ts) {
    ReturnVisitor<long> return_visit(return_policy::monetary);
    // df.template sort<long>("time", sort_spec::ascen);
    df.template single_act_visit<long>("time", return_visit);
    df.template load_column<long>("time_diff", return_visit.get_result());

    FactorizeVisitor<long> fact(
        [](const long& f) -> bool { return (f > -600); });
    df.template load_column(
        "time_diff_bool",
        df.template single_act_visit<long>("time_diff", fact).get_result());
    auto session_df = df.template groupby1<bool>(
        "time_diff_bool",
        LastVisitor<MyDataFrame::IndexType, MyDataFrame::IndexType>(),
        std::make_tuple("time_diff", "all_session_time", SumVisitor<long>()),
        std::make_tuple("time_diff", "session_num", CountVisitor<long>()));
    // session_df.template write<std::ostream, int, long, bool, size_t,
    // std::string>(std::cout, io_format::csv2);

    auto event_list = session_df.template get_column<bool>("time_diff_bool");
    auto event_num = (long)event_list.size();
    auto session_num_14d =
        session_df.template get_column<size_t>("session_num")[0];
    auto all_session_time =
        session_df.template get_column<long>("all_session_time")[1];
    auto avg_session_minute = abs(all_session_time) / 60 / session_num_14d;
    auto avg_imp_in_session = (float)event_num / session_num_14d;
    // latest_session_time decided by time_diff between req_ts and cur_ts
    // auto latest_session_time = session_df.template get_column<long>
    // ("time_diff")[0];

    fmt::print("session_num={}; all_session_time={}, avg_session_minute={}, "
               "avg_imp_in_session={} \n",
               session_num_14d,
               all_session_time,
               avg_session_minute,
               avg_imp_in_session);
}
// click funcs
template <typename I, typename H>
void extractImpNotCli(DataFrame<I, H>& df_clk, DataFrame<I, H>& df_imp) {
    // calculate imp_not_click_pkgs
    std::cout << "in func: extractImpNotCli"
              << "\n";
    // df_clk.template write<std::ostream, long, std::string>(std::cout,
    // io_format::csv2); problem1: segment fault
    auto cli_pkg_vec = df_clk.template get_column<std::string>("pkg");
    // std::for_each(cli_pkg_vec.begin(), cli_pkg_vec.end(), [](const auto
    // &i){std::cout << i << " "; });
    auto imp_pkg_vec = df_imp.template get_column<std::string>("pkg");
    for (auto it = imp_pkg_vec.begin(); it != imp_pkg_vec.end(); it++) {
        if (std::find(cli_pkg_vec.begin(), cli_pkg_vec.end(), *it) !=
            cli_pkg_vec.end()) {
            imp_pkg_vec.erase(it);
        }
    }
    std::cout << "imp_not_click:";
    for (auto& i : imp_pkg_vec) {
        std::cout << i << ",";
    }
    std::cout << "\n";
}

template <typename I, typename H>
void extractCtTopN(DataFrame<I, H>& df) {
    auto time_vec = df.template get_column<long>("time");
    for (auto& i : time_vec) {
        i = (i / 3600) % 24;
    }
    df.template load_column<long>("hour_of_day", time_vec);
    ModeVisitor<1, long> mode_visit;
    const auto&          result =
        df.template single_act_visit<long>("hour_of_day", mode_visit)
            .get_result();
    std::cout << "ctTop3:";
    for (auto& i : result) {
        std::cout << i.get_value() << ",";
    }
    std::cout << "\n";
}

template <typename I, typename H>
int CalcImpUpsFea(DataFrame<I, H>& df) {
    // check time column
    auto dvec = df.template get_column<std::string>("time");
    std::for_each(
        dvec.begin(), dvec.end(), [](const auto& i) { std::cout << i << " "; });
    std::cout << "\n";

    const auto p1 = std::chrono::system_clock::now();
    long       req_ts = (long)std::chrono::duration_cast<std::chrono::seconds>(
                      p1.time_since_epoch())
                      .count();
    req_ts = 1622736000;

    df.template retype_column<std::string, long>(
        "time",
        [](const std::string& val) -> long { return (std::stol(val)); });

    extractSessionFea(df, req_ts);
    extractEventNumFea(df, req_ts);
    auto imp_pkg_num = (int)dvec.size();
    fmt::print("imp_pkg_num={}\n", imp_pkg_num);

    return 0;
}

template <typename I, typename H>
int CalcInsUpsFea(DataFrame<I, H>& df_ins) {
    const auto p1 = std::chrono::system_clock::now();
    long       req_ts = (long)std::chrono::duration_cast<std::chrono::seconds>(
                      p1.time_since_epoch())
                      .count();
    req_ts = 1624118400;

    df_ins.template retype_column<std::string, long>(
        "time",
        [](const std::string& val) -> long { return (std::stol(val)); });
    auto dvec = df_ins.template get_column<long>("time");

    extractTopNFea(df_ins, "Ins");
    auto df_ins_24h = extractTopNFeaByTime(df_ins, req_ts, "Ins");
    extractEventNumFea(df_ins, req_ts);

    // extractCtTopN(df_clk);
    auto ins_pkg_num = (int)dvec.size();
    fmt::print("ins_pkg_num={}\n", ins_pkg_num);
    auto pkg_vec = df_ins.template get_column<std::string>("pkg");
    auto last_ip = pkg_vec[0];
    fmt::print("last_ip={}\n", last_ip);
    unsigned long last_ip_time_diff = req_ts - dvec[0];
    fmt::print("last_ip_time_diff={}\n", last_ip_time_diff);
    return 0;
}

template <typename I, typename H>
int CalcCliUpsFea(DataFrame<I, H>& df_clk, DataFrame<I, H>& df_imp) {
    // check time column
    auto dvec = df_clk.template get_column<std::string>("time");

    const auto p1 = std::chrono::system_clock::now();
    long       req_ts = (long)std::chrono::duration_cast<std::chrono::seconds>(
                      p1.time_since_epoch())
                      .count();
    req_ts = 1624118400;

    df_clk.template retype_column<std::string, long>(
        "time",
        [](const std::string& val) -> long { return (std::stol(val)); });
    // problem1: mode_visit直接传3会有问题
    extractTopNFea(df_clk, "Cli");
    auto df_clk_24h = extractTopNFeaByTime(df_clk, req_ts, "Cli");
    if (dvec.size() > 3) {
        extractEventNumFea(df_clk, req_ts);
        extractCtTopN(df_clk);
        // problem2: 某几条数据在取pkg时会segment fault
        // extractImpNotCli(df_clk, df_imp);
        auto clk_pkg_num = (int)dvec.size();
        fmt::print("clk_pkg_num={}\n", clk_pkg_num);
        // pkg 24h uniq
        auto cli_pkg_vec_24h =
            df_clk_24h.template get_col_unique_values<std::string>("pkg");
        std::cout << "pkg_seq_24_hour_uniq:";
        for (auto& i : cli_pkg_vec_24h) {
            std::cout << i << ",";
        }
        std::cout << "\n";
    }
    return 0;
}

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
    auto thr = 10000;
    auto fn = [thr](const unsigned long&, const long& val) {
        return val > thr;
    };
    auto df_view =
        df.template get_view_by_sel<long, decltype(fn), long, std::string>(
            "appid", fn);
    df_view.template write<std::ostream, int, long, double, std::string>(
        std::cout);
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

    // check time column
    auto dvec = df.template get_column<std::string>("time");
    std::for_each(
        dvec.begin(), dvec.end(), [](const auto& i) { std::cout << i << " "; });
    std::cout << "\n";

    const auto p1 = std::chrono::system_clock::now();
    long       req_ts = (long)std::chrono::duration_cast<std::chrono::seconds>(
                      p1.time_since_epoch())
                      .count();
    req_ts = 1623159350;

    df.template retype_column<std::string, long>(
        "time",
        [](const std::string& val) -> long { return (std::stol(val)); });

    extractTopNFea(df);
    extractTopNFeaByTime(df, req_ts); // also modeVisitor
    // extractSessionFea(df, req_ts);
    // extractEventNumFea(df, req_ts);

    auto imp_pkg_num = (int)dvec.size();
    fmt::print("imp_pkg_num={}", imp_pkg_num);
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