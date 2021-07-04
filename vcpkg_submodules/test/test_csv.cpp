#include <csv.h>
#include <dbg.h>
#include <fmt/format.h>
#include <fstream>
#include <string>
template <int depth>
struct Bit_patterns;

// specializations for certain types
template <>
struct Bit_patterns<sizeof(uint32_t)> {
    static constexpr uintptr_t newline_value = 0x0a0a0a0a;
    static constexpr uintptr_t add_mask = 0x7efefeff;
    static constexpr uintptr_t and_mask = 0x81010100;
};

template <>
struct Bit_patterns<sizeof(uint64_t)> {
    static constexpr uintptr_t newline_value = 0x0a0a0a0a0a0a0a0a;
    static constexpr uintptr_t add_mask = 0x7efefefefefefeff;
    static constexpr uintptr_t and_mask = 0x8101010101010100;
};

constexpr int get_chars_in_line(const char* buffer, const char* buffer_end) {
    const auto buffer_start = buffer;

    if ((buffer + sizeof(uintptr_t)) < buffer_end) {
        uintptr_t data = 0;
        while ((data == 0) && (buffer + sizeof(uintptr_t)) < buffer_end) {
            memcpy(&data, buffer, sizeof(data));

            // this is an adaption of the following bithack
            // -> http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
            // normalize the bits read, so that the newline character is
            // transformed to zero and all other values to non-zero
            data ^= Bit_patterns<sizeof(data)>::newline_value;

            auto helper = data;
            data += Bit_patterns<sizeof(data)>::add_mask;
            data ^= ~helper;
            data &= Bit_patterns<sizeof(data)>::and_mask;

            buffer += sizeof(uintptr_t);
        }

        buffer -= sizeof(uintptr_t);
    }

    while (buffer != buffer_end && *buffer != '\n') {
        ++buffer;
    }

    return buffer - buffer_start;
}

std::string ReadAll(const std::string& filename) {
    std::ifstream ifs(filename);
    std::string   content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    return std::move(content);
}

// #define CSV_IO_NO_THREAD
int main(int argc, char const* argv[]) {
    // 3是有几列，括号里面是文件名
    std::string      filename = "../data/ram.csv";
    io::CSVReader<3> in(filename);
    bool             read_header = false;
    if (read_header) {
        in.read_header(io::ignore_extra_column, "vendor", "size", "speed");
    }
    else {
        in.next_line();
    }
    std::string vendor;
    int         size;
    double      speed;
    //注意，上面的变量目前只支持c++自带的一些类型，可以看
    while (in.read_row(vendor, size, speed)) {
        fmt::print("{} {} {}\n", vendor, size, speed);
    }
    auto           csv_str = ReadAll(filename);
    io::LineReader file_reader(
        filename, csv_str.data(), csv_str.data() + csv_str.size());
    char* line = nullptr;
    while ((line = file_reader.next_line()) != nullptr) {
        fmt::print("{}\n", line);
    }
    return 0;
}