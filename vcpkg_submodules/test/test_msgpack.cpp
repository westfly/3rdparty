#include <fmt/format.h>
#include <iostream>
#include <msgpack.hpp>
#include <sstream>
#include <string>

void test_vector() {
    // serializes this object.
    std::vector<std::string> vec;
    vec.push_back("Hello");
    vec.push_back("MessagePack");

    // serialize it into simple buffer.
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    // deserialize it.
    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

    // print the deserialized object.
    msgpack::object obj = oh.get();
    std::cout << obj << std::endl; //=> ["Hello", "MessagePack"]

    // convert it into statically typed object.
    std::vector<std::string> rvec;
    obj.convert(rvec);
}
struct Foo {
    int         i;
    std::string str;
    // 原始指针类型，内部封装了pack_raw和pack_raw_body方法
    msgpack::type::raw_ref data;

public:
    MSGPACK_DEFINE(i, str, data);
};
int test_struct() {

    using namespace std;
    Foo f;
    f.i = 4;
    f.str = "hello world";
    const char* tmp = "msgpack";
    f.data.ptr = tmp;
    f.data.size = strlen(tmp) + 1;

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, f);

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

    msgpack::object obj = oh.get();

    Foo f2;
    obj.convert(f2);
    cout << f2.i << ", " << f2.str << ", ";
    cout << f2.data.ptr << endl;

    return 0;
}
int test_tuple() {
    msgpack::type::tuple<int, bool, std::string> src(1, true, "example");

    // serialize the object into the buffer.
    // any classes that implements write(const char*,size_t) can be a buffer.
    std::stringstream buffer;
    msgpack::pack(buffer, src);

    // send the buffer ...
    buffer.seekg(0);

    // deserialize the buffer into msgpack::object instance.
    std::string str(buffer.str());

    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    // deserialized object is valid during the msgpack::object_handle instance
    // is alive.
    msgpack::object deserialized = oh.get();

    // msgpack::object supports ostream.
    fmt::print("{}\n", str);
    // msgpack::object supports ostream.
    std::cout << deserialized << std::endl;

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    msgpack::type::tuple<int, bool, std::string> dst;
    deserialized.convert(dst);

    // or create the new instance
    msgpack::type::tuple<int, bool, std::string> dst2 =
        deserialized.as<msgpack::type::tuple<int, bool, std::string>>();

    return 0;
}
int main(void) {
    test_tuple();
    test_vector();
    test_struct();
    return 0;
}