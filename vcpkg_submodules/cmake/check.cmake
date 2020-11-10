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