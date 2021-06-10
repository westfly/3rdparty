#pragma once
#include "static_reflection.h"
struct SimpleStruct {
    bool        bool_;
    int         int_;
    double      double_;
    std::string string_;
    bool        optional_;
    // std::unique_ptr<bool> optional_;
};

struct NestedStruct {
    SimpleStruct              nested_;
    std::vector<SimpleStruct> vector_;
};
DEFINE_STRUCT_SCHEMA(SimpleStruct,
                     DEFINE_STRUCT_FIELD(bool_, "_bool"),
                     DEFINE_STRUCT_FIELD(int_, "_int"),
                     DEFINE_STRUCT_FIELD(double_, "_double"),
                     DEFINE_STRUCT_FIELD(string_, "_string"),
                     DEFINE_STRUCT_FIELD(optional_, "_optional"));

DEFINE_STRUCT_SCHEMA(NestedStruct,
                     DEFINE_STRUCT_FIELD(nested_, "_nested"),
                     DEFINE_STRUCT_FIELD(vector_, "_vector"));

struct Feature {
    int    id;
    double score;
#if 0
    bool   operator<(const Feature& b) const {
        return score <= b.score;
    }
    bool operator>(const Feature& b) const {
        return score > b.score;
    }
#else
    auto operator<=>(const Feature& that) const {
        return score <=> that.score;
    }
#endif
};
std::ostream& operator<<(std::ostream& out, const Feature& v) {
    out << "id " << v.id << "\tscore " << v.score << std::endl;
    return out;
}

// Improve
#define HAS_MEMBER(member)                                                     \
    template <typename T, typename... Args>                                    \
    struct has_member_##member {                                               \
    private:                                                                   \
        template <typename U>                                                  \
        static auto Check(int)                                                 \
            -> decltype(std::declval<U>().member(std::declval<Args>()...),     \
                        std::true_type());                                     \
        template <typename U>                                                  \
        static std::false_type Check(...);                                     \
                                                                               \
    public:                                                                    \
        enum {                                                                 \
            value = std::is_same<decltype(Check<T>(0)), std::true_type>::value \
        };                                                                     \
    };
