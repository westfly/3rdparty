// from https://github.com/felixguendling/cista
// make cista-test-single-header
#include "cista.h"
#include <fmt/format.h>
#include <rapidcsv.h>
#define DBG_MACRO_NO_WARNING 1
#include "utility.hpp"
#include <dbg.h>
#include <mio/mmap.hpp>
namespace rapidcsv {
template <>
void Converter<std::string_view>::ToVal(const std::string& pStr,
                                        std::string_view&  pVal) const {
    pVal = std::string_view(pStr.data(), pStr.size());
}
} // namespace rapidcsv
struct MutipleCell {
    int                      val;
    std::vector<std::string> col;
    CISTA_PRINTABLE(MutipleCell);
};
void ConvertMutipleCell(const std::string& pStr, MutipleCell& pVal) {
    auto val = static_cast<int>(roundf(100.0f * std::stof(pStr)));
    pVal.val = val;
    // Just Test
    for (int i = 0; i < 3; ++i) {
        pVal.col.push_back(std::to_string(val));
    }
}
#define XConvertTypeOfDocument(cpp_type, fn, ...)                              \
    do {                                                                       \
        auto col = doc.Get##fn<cpp_type>(__VA_ARGS__);                         \
        dbg("ConvertType {} {}", #cpp_type, col);                              \
    } while (0)

int RapidCSV() {
    {
        rapidcsv::Document doc("../data/colhdr.csv");
#define XConvertTypeColumnClose(cpp_type)                                      \
    XConvertTypeOfDocument(cpp_type, Column, "Close")

        XConvertTypeColumnClose(float);
        XConvertTypeColumnClose(std::string_view);
    }
    {
        rapidcsv::Document doc("../data/colrowhdr.csv",
                               rapidcsv::LabelParams(0, 0));
        dbg("get{}", doc.GetColumnNames());
        std::vector<float> close = doc.GetRow<float>("2017-02-22");
        std::cout << "Read " << close.size() << " values." << std::endl;
        dbg(close);
#define XConvertTypeCellVolume(cpp_type)                                       \
    XConvertTypeOfDocument(cpp_type, Cell, "Volume", "2017-02-22");
        XConvertTypeCellVolume(std::string);
        XConvertTypeCellVolume(std::string_view);
        XConvertTypeCellVolume(int);
        XConvertTypeCellVolume(long);
        XConvertTypeCellVolume(long long);
        XConvertTypeCellVolume(unsigned);
        XConvertTypeCellVolume(unsigned int);
        XConvertTypeCellVolume(unsigned long);
        XConvertTypeCellVolume(unsigned long long);
        XConvertTypeCellVolume(float);
        XConvertTypeCellVolume(double);
        XConvertTypeCellVolume(long double);
        XConvertTypeCellVolume(char);
        XConvertTypeOfDocument(
            MutipleCell, Cell, "Close", "2017-02-21", ConvertMutipleCell);
    }
    {
        rapidcsv::Document doc("../data/rowhdr.csv",
                               rapidcsv::LabelParams(-1, 0));
#define XConvertTypeRow(cpp_type)                                              \
    XConvertTypeOfDocument(cpp_type, Row, "2017-02-22");
        XConvertTypeRow(std::string);
        XConvertTypeRow(float);
    }
    {
        rapidcsv::Document doc("../data/nohdr.csv",
                               rapidcsv::LabelParams(-1, -1),
                               rapidcsv::SeparatorParams(','));

        XConvertTypeOfDocument(float, Column, 5);
        XConvertTypeOfDocument(long long, Cell, 4, 2);
    }
    return 0;
}
int MIO() {
    using mmap_source = mio::basic_mmap_source<std::byte>;
    std::error_code  error;
    auto             path = "../data/rowhdr.csv";
    mio::mmap_source ro_mmap;
    ro_mmap.map(path, error);
    return 0;
}
void TestConverter() {
    auto        convert = utils::Converter<int>();
    std::string abc = "123";
    int         val;
    convert.ToVal(abc, val);
    fmt::print("abc {} to {}\n", abc, val);
}
int main(int argc, char const* argv[]) {
    RapidCSV();
    MIO();
    TestConverter();
    return 0;
}
