project(ImportAsExternalProject)
cmake_minimum_required(VERSION 3.14)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0012 NEW)
endif(COMMAND cmake_policy)
if(POLICY CMP0003)
    cmake_policy(SET CMP0003 NEW)
endif()
if(POLICY CMP0012)
    cmake_policy(SET CMP0012 NEW)
endif()
set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)
set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")
message("${CMAKE_CURRENT_SOURCE_DIR}@ ${CMAKE_TOOLCHAIN_FILE}")

find_path(DBG_MACRO_INCLUDE_DIRS "dbg.h")

macro(add_test_library name libname)
    find_package(${libname} CONFIG REQUIRED)
    set(target_name test_${name})
    add_executable(
        ${target_name}
        test/test_${name}.cpp
    )
    if(${ARGC} GREATER 2)
        set(link_target ${ARGN})
    else()
        set(link_target ${libname}::${libname})
    endif()
    message("define ${ARGV2} ${link_target}")
    target_link_libraries(
        ${target_name} PRIVATE
        ${link_target}
    )
endmacro(add_test_library)

add_test_library(spdlog spdlog)
add_test_library(scnlib scn scn::scn spdlog::spdlog)
add_test_library(fmt fmt)
add_test_library(quill quill)
find_package(GTest CONFIG REQUIRED)
add_test_library(folly folly GTest::gtest_main Folly::folly)
add_test_library(xtensor xtensor xtensor)
add_test_library(catch Catch2)
add_test_library(json11 fmt fmt::fmt json11)
add_test_library(msgpack msgpack msgpackc msgpackc-cxx fmt::fmt)
add_test_library(cuckoo libcuckoo libcuckoo)
add_test_library(abseil absl absl::any absl::base absl::strings fmt::fmt)
#add_test_library(macro boost)
macro(add_test_library_include name libname)
    find_package(${libname} CONFIG REQUIRED)
    set(target_name test_${name})
    add_executable(
        ${target_name}
        test/test_${name}.cpp
    )
    string(TOUPPER ${name} UPPER_NAME)
    set(include_target ${UPPER_NAME}_INCLUDE_DIRS)
    target_include_directories(
        ${target_name} PRIVATE ${${UPPER_NAME}_INCLUDE_DIRS}
    )

    set(link_target ${UPPER_NAME}_LIBRARIES)
    target_link_libraries(
        ${target_name} PRIVATE ${${UPPER_NAME}_LIBRARIES}
    )
    message("############## echo ${include_target}  ${link_target} ")
endmacro(add_test_library_include)

macro(add_test_header_only_library name libname)
    set(target_name test_${name})
    add_executable(
        ${target_name}
        test/test_${name}.cpp
    )
    string(TOUPPER ${name} UPPER_NAME)
    find_path(${UPPER_NAME}_INCLUDE_DIRS "${name}.h")
    target_include_directories(
        ${target_name} PRIVATE ${${UPPER_NAME}_INCLUDE_DIRS}
    )
    target_link_libraries(
        ${target_name} PRIVATE
        ${ARGV2}
    )
endmacro(add_test_header_only_library)


add_test_library_include(sockpp sockpp)
add_test_library_include(rapidjson RapidJSON)
add_test_header_only_library(nanobench nanobench)
add_test_header_only_library(cpptoml cpptoml spdlog::spdlog)



find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(test_pybind11 test/test_pybind11.cpp)
pybind11_add_module(cashflow test/cashflow.cpp)
