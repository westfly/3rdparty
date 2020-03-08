get_filename_component(DEP_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(TOUPPER ${DEP_NAME} UP_DEP_NAME)
message("${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR} ${DEP_NAME}")
set(GIT_URL jarro2783/cxxopts)
set(GIT_TAG v2.2.0)
set(DEP_PREFIX ${CMAKE_CURRENT_LIST_DIR})
message("Install @ ${DEP_PREFIX}")
set(DEP_SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/${DEP_NAME}-src)
message("Install @ ${DEP_SOURCE_DIR}")
set(CMAKE_VERBOSE_MAKEFILE ON)
CPMAddPackage(
  NAME              ${DEP_NAME}
  GITHUB_REPOSITORY ${GIT_URL}
  GIT_SHALLOW    	TRUE
  VERSION           ${GIT_TAG}
  SOURCE_DIR		${DEP_SOURCE_DIR}
  LOG_DOWNLOAD      ON
  QUIET				OFF
  CONFIGURE_COMMAND "cd ${DEP_SOURCE_DIR} && cmake -D CMAKE_INSTALL_PREFIX=${DEP_PREFIX} ."
  BUILD_COMMAND     "cd ${DEP_SOURCE_DIR} && make -j30"
  INSTALL_COMMAND   "cd ${DEP_SOURCE_DIR} && make install"
)


if(EXISTS ${DEP_PREFIX}/lib)
	set(${UP_DEP_NAME}_LIB ${DEP_PREFIX}/lib/lib${DEP_NAME}.a)
endif()
if(EXISTS ${DEP_PREFIX}/lib64)
	set(${UP_DEP_NAME}_LIB ${DEP_PREFIX}/lib64/lib${DEP_NAME}.a)
endif()
if(EXISTS ${_DEP_PREFIX}/include)
	set(${UP_DEP_NAME}_INCLUDE_DIR   ${DEP_PREFIX}/include)
endif()

message("${DEP_PREFIX}/include ${DEP_PREFIX}")

mark_as_advanced(
  ${UP_DEP_NAME}_LIBRARIES
)

