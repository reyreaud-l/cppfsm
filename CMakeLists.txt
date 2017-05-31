cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME_STR cppfsm)
project(${PROJECT_NAME_STR} CXX)

#CXX FLAGS
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++11 -pedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


#CPPFSM
set(COMMON_INCLUDES include/)

#TESTS
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${COMMON_INCLUDES} ${PROJECT_SOURCE_DIR}/ext/)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cc)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})

add_custom_target(
  check
  DEPENDS ${PROJECT_TEST_NAME}
  COMMAND ./${PROJECT_TEST_NAME}
)
