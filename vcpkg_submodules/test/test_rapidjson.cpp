#include <string>
#include <iostream>
const std::string kJson = R"({
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
    document.Parse(kJson.data());
    const char* str = kJson.data();
    using namespace std::literals;
    auto key = "hello";
    assert(document.HasMember(key));
    assert(document[key].IsString());
    auto sv = document[key].GetString();
    printf("\nhello = %s\t %p %p %ld\n", sv, sv, str, sv - str);
    return 0;
}