get_filename_component(DEP_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(TOUPPER ${DEP_NAME} UP_DEP_NAME)
message("${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR} ${DEP_NAME}")
set(GIT_URL gabime/spdlog)
set(GIT_TAG 1.5.0)
set(DEP_PREFIX ${CMAKE_CURRENT_LIST_DIR})
message("Install @ ${DEP_PREFIX}")
set(DEP_SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/${DEP_NAME}-src)
message("Install @ ${DEP_SOURCE_DIR}")
set(CMAKE_VERBOSE_MAKEFILE ON)
# cmake-format: off
CPMAddPackage(
  NAME              ${DEP_NAME}
  GITHUB_REPOSITORY ${GIT_URL}
  GIT_SHALLOW    	  TRUE
  VERSION           ${GIT_TAG}
  SOURCE_DIR		    ${DEP_SOURCE_DIR}
  LOG_DOWNLOAD      ON
  QUIET				      OFF
  CONFIGURE_COMMAND "cd ${DEP_SOURCE_DIR} && cmake -D CMAKE_INSTALL_PREFIX=${DEP_PREFIX} ."
  BUILD_COMMAND     "cd ${DEP_SOURCE_DIR} && make -j30"
  INSTALL_COMMAND   "cd ${DEP_SOURCE_DIR} && make install"
)
# cmake-format: on
message("########### ${DEP_PREFIX} ${DEP_NAME} ${UP_DEP_NAME}")
#library_env_setup(${DEP_PREFIX} ${DEP_NAME} ${UP_DEP_NAME})
