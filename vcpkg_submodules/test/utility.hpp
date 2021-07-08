#include <exception>
#include <string>
#include <type_traits>
namespace utils {

// https://stackoverflow.com/questions/13101061/detect-if-a-type-is-a-stdtuple
/*
if constexpr (is_tuple<Type>::value) {
}
*/
template <typename>
struct is_tuple : std::false_type {};

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};
class no_converter : public std::exception {
    /**
     * @brief   Provides details about the exception
     * @returns an explanatory string
     */
    virtual const char* what() const throw() {
        return "unsupported conversion datatype";
    }
};
template <typename T>
class Converter {
public:
    void ToStr(const T& pVal, std::string& pStr) const {
        throw no_converter();
    }
    void ToVal(const std::string& pStr, T& pVal) const {
        throw no_converter();
    }
};
#define ConverterImpl(cpp_type, ToString, ToValue)                             \
    template <>                                                                \
    class Converter<cpp_type> {                                                \
    public:                                                                    \
        void ToStr(const cpp_type& pVal, std::string& pStr) const {            \
            pStr = ToString(pVal);                                             \
        }                                                                      \
        void ToVal(const std::string& pStr, cpp_type& pVal) const {            \
            pVal = static_cast<cpp_type>(ToValue(pStr));                       \
        }                                                                      \
    };
ConverterImpl(int, std::to_string, std::stoi);
ConverterImpl(long, std::to_string, std::stol);
ConverterImpl(long long, std::to_string, std::stoll);
ConverterImpl(unsigned long, std::to_string, std::stoul);
ConverterImpl(unsigned long long, std::to_string, std::stoull);
ConverterImpl(float, std::to_string, std::stof);
ConverterImpl(double, std::to_string, std::stod);
ConverterImpl(long double, std::to_string, std::stold);
} // namespace utils