macro(add_test_library name libname)
    set(target_name test_${name})
    add_executable(
        ${target_name}
        test/test_${name}.cpp
    )
    target_link_libraries(
        ${target_name} PRIVATE
        ${libname}
    )
endmacro(add_test_library)
function(debug_function)
    message(STATUS "ARGN: ${ARGN}")
    message(STATUS "ARGC: ${ARGC}")
    message(STATUS "ARGV: ${ARGV}")
    message(STATUS "ARGV0: ${ARGV0}")

    list(LENGTH ARGV  argv_len)
    message(STATUS "length of ARGV: ${argv_len}")
    set(i 0)
    while( i LESS ${argv_len})
         list(GET ARGV ${i} argv_value)
         message(STATUS "argv${i}: ${argv_value}")
         math(EXPR i "${i} + 1")
    endwhile()
endfunction(debug_function)

function(library_env_setup dep_prefix dep_name up_dep_name)
  #debug_function(${dep_prefix} ${dep_name} ${up_dep_name} ${ARGN})
  if(EXISTS ${dep_prefix}/lib)
    set(${up_dep_name}_lib ${dep_prefix}/lib/lib${dep_name}.a)
  endif()
  if(exists ${dep_prefix}/lib64)
    set(${up_dep_name}_lib ${dep_prefix}/lib64/lib${dep_name}.a)
  endif()
  if(exists ${dep_prefix}/include)
    set(${up_dep_name}_include_dir   ${dep_prefix}/include)
  endif()
  message("${dep_prefix}/include ${dep_prefix}")
  mark_as_advanced(
    ${up_dep_name}_libraries
  )
endfunction(library_env_setup)