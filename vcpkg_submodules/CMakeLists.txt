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

set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")
message("${CMAKE_CURRENT_SOURCE_DIR} @ ${CMAKE_TOOLCHAIN_FILE}")

project(ImportAsExternalProject)
cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-std=c++20)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/check.cmake)
add_required_library(fmt scn RapidJSON)
find_path(DBG_MACRO_INCLUDE_DIRS "dbg.h")
add_test_library(dataframe DataFrame fmt::fmt DataFrame::DataFrame ${RAPIDJSON_INCLUDE_DIRS} pthread)
