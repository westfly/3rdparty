#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <fmt/format.h>
class my_class {
public:
    my_class(const std::string& name, int age) {
        m_age = age;
        m_name = name;
    }

public:
    int get_age() {
        return m_age;
    }
    std::string get_name() {
        return m_name;
    }
    std::string DebugString() const {
        return fmt::format("age {} name {}", m_age, m_name);
    }
private:
    int         m_age;
    std::string m_name;
};

TEST(test_my_class, get_age) {
    my_class myClass("Joel", 21);
    ASSERT_TRUE(myClass.get_age() == 21) << "age is not 16";
}

TEST(test_my_class, get_name) {
    my_class myClass("Joel", 21);
    ASSERT_EQ(myClass.get_name(), "Joel") << "name is not Joel";
}

TEST(test_my_class, hash_move) {
    std::unordered_map<int,my_class> map;
    map.try_emplace(1, "Joel", 21);
    auto iter = map.find(1);
    auto p = std::move(iter->second);
    fmt::print("{}\n", p.DebugString());
    iter = map.find(1);
    fmt::print("{}\n", iter->second.DebugString());
}