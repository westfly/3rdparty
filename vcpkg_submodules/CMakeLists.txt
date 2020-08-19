project(ImportAsExternalProject)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")
message("${CMAKE_CURRENT_SOURCE_DIR}@ ${CMAKE_TOOLCHAIN_FILE}")
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
add_test_library(scnlib scn)
add_test_library(fmt fmt)
add_test_library(quill quill)
find_package(GTest CONFIG REQUIRED)
add_test_library(folly folly GTest::gtest_main Folly::folly)
add_test_library(xtensor xtensor xtensor)
