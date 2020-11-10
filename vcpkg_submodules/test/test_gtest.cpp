#include <gtest/gtest.h>

#include <string>

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

private:
    int         m_age;
    std::string m_name;
};

TEST(test_my_class, get_age) {
    my_class myClass("Joel", 21);
    ASSERT_TRUE(myClass.get_age() == 16) << "age is not 16";
}

TEST(test_my_class, get_name) {
    my_class myClass("Joel", 21);
    ASSERT_EQ(myClass.get_name(), "Joel") << "name is not Joel";
}