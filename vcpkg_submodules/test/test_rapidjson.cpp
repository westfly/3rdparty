#include <string>
#include <iostream>
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

// ...

int main()
{
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
        printf("\n%s\t === %d\n", iter->value.GetString(), iter->value.GetStringLength());
    }

    return 0;
}