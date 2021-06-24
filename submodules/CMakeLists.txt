project(ImportAsExternalProject)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)
macro(add_test_library name libname)
    add_subdirectory(${name} EXCLUDE_FROM_ALL)
    set(target_name test_${name})
    add_executable(
        ${target_name}
        test/test_${name}.cpp
    )
    target_link_libraries(
        ${target_name} PRIVATE
        ${libname}::${libname}
    )
endmacro(add_test_library)
add_test_library(spdlog spdlog)
add_test_library(scnlib scn)
add_test_library(fmt fmt)
add_test_library(quill quill)
add_test_library(cista cista)
