#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int a, int b) {
    return a + b;
}
PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example module";
    // Add bindings here
    m.def("foo", []() { return "Hello, World!"; });
    m.def("add", &add, "A function which adds two numbers");
}
