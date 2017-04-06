cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME_STR cppfsm)
project(${PROJECT_NAME_STR} CXX)

#CXX FLAGS
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++11 -pedantic -g -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


#CPPFSM
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_lib)
set(COMMON_INCLUDES src/)
file(GLOB LIB_SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)
add_library(
  ${PROJECT_LIB_NAME}
  SHARED
  ${LIB_SRC_FILES}
  ${COMMON_INCLUDES}
)

#GTEST
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

#TESTS
enable_testing()
find_package(Threads REQUIRED)
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${COMMON_INCLUDES}
)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cc)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME})
target_link_libraries(${PROJECT_TEST_NAME}
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
)
target_link_libraries(${PROJECT_TEST_NAME}
  ${GMOCK_LIBS_DIR}/libgmock.a
  ${GMOCK_LIBS_DIR}/libgmock_main.a
)

add_custom_target(
  check
  DEPENDS ${PROJECT_TEST_NAME}
  COMMAND ./${PROJECT_TEST_NAME}
)
