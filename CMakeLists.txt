cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++14 -pedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SRC_LIB
  src/fsm.cc
  src/transition.cc
  src/state.cc
  src/misc.cc
)

include_directories(src/)

add_library(
  "cppfsm"
  SHARED
  ${SRC_LIB}
  ${INCLUDE_LIB}
)

add_executable(
  testme tests/main.cc
)

target_link_libraries(
  testme cppfsm
)

add_custom_target(
  check
  DEPENDS testme
  COMMAND ./testme
  COMMAND rm -f testme
)
