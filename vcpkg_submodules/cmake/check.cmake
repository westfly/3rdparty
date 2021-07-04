macro(check_target_exist target_name name)
    if(NOT TARGET ${target_name})
        add_executable(
            ${target_name}
            test/test_${name}.cpp
        )
    else()
        message("####### ${target_name} exist")
    endif()
endmacro(check_target_exist)

macro(add_required_library)
    foreach(libname ${ARGN})
        find_package(${libname} CONFIG REQUIRED)
        message("add ${libname}")
    endforeach()
endmacro(add_required_library)

macro(add_headonly_required_library)
    foreach(name ${ARGN})
        string(TOUPPER ${name} UPPER_NAME)
        find_path(${UPPER_NAME}_INCLUDE_DIRS "${name}.hpp")
        message("headonly add ${name} ${UPPER_NAME}")
    endforeach()
endmacro(add_headonly_required_library)
macro(add_test_library_include name libname)
    find_package(${libname} CONFIG REQUIRED)
    set(target_name test_${name})
    check_target_exist(${target_name} ${name})
    string(TOUPPER ${name} UPPER_NAME)
    set(include_target ${UPPER_NAME}_INCLUDE_DIRS)
    if (NOT ${include_target} MATCHES  "NOTFOUND$")
        target_include_directories(
            ${target_name} PRIVATE ${${UPPER_NAME}_INCLUDE_DIRS}
        )
    endif()
    set(link_target ${UPPER_NAME}_LIBRARIES)
    if (NOT ${UPPER_NAME}_LIBRARIES} MATCHES "NOTFOUND$")
        target_link_libraries(
            ${target_name} PRIVATE ${${UPPER_NAME}_LIBRARIES}
        )
    endif()
    message("############## echo ${${include_target}} \n ##### ${${link_target}} ")
endmacro(add_test_library_include)

macro(add_test_header_only_library name libname)
    set(target_name test_${name})
    check_target_exist(${target_name} ${name})
    string(TOUPPER ${name} UPPER_NAME)
    find_path(${UPPER_NAME}_INCLUDE_DIRS "${name}.h")
    target_include_directories(
        ${target_name} PRIVATE ${${UPPER_NAME}_INCLUDE_DIRS}
    )
    target_link_libraries(
        ${target_name} PRIVATE
        ${ARGN}
    )
endmacro(add_test_header_only_library)

macro(add_test_library name libname)
    find_package(${libname} CONFIG REQUIRED)
    set(target_name test_${name})
    check_target_exist(${target_name} ${name})
    if(${ARGC} GREATER 2)
        set(link_target ${ARGN})
    else()
        set(link_target ${libname}::${libname})
    endif()
    message("define ${target_name} ${libname} ${ARGV2} link_target ${link_target}")
    target_link_libraries(
        ${target_name} PRIVATE
        ${link_target}
    )
endmacro(add_test_library)