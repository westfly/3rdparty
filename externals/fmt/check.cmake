get_filename_component(DEP_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(TOUPPER ${DEP_NAME} UP_DEP_NAME)
message("${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR} ${DEP_NAME}")
set(GIT_URL fmtlib/fmt)
set(GIT_TAG 6.1.2)
set(GIT_TAG master)
set(DEP_PREFIX ${CMAKE_CURRENT_LIST_DIR})
message("Install @ ${DEP_PREFIX}")
set(DEP_SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/${DEP_NAME}-src)
message("Install @ ${DEP_SOURCE_DIR}")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CPM_ARGS_GIT_TAG ${GIT_TAG})
CPMAddPackage(
  NAME              ${DEP_NAME}
  GITHUB_REPOSITORY ${GIT_URL}
  GIT_SHALLOW    	TRUE
  GIT_TAG           ${GIT_TAG} # not pass version
  CPM_ARGS_GIT_TAG  ${GIT_TAG}
  SOURCE_DIR		${DEP_SOURCE_DIR}
  LOG_DOWNLOAD      ON
  QUIET				OFF
  CONFIGURE_COMMAND "cd ${DEP_SOURCE_DIR} && cmake -D CMAKE_INSTALL_PREFIX=${DEP_PREFIX} ."
  BUILD_COMMAND     "cd ${DEP_SOURCE_DIR} && make -j30"
  INSTALL_COMMAND   "cd ${DEP_SOURCE_DIR} && make install"
)
