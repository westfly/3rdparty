#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <dbg.h>
namespace py = pybind11;
int main() {
    py::scoped_interpreter python;
    py::module t = py::module::import("example");
    t.attr("add")(1, 2);
    return 0;
}