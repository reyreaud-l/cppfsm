#define CATCH_CONFIG_MAIN //Catch provide a main
#define __TEST_CPPFSM 
#include "catch.hpp"
#include "test.hh"
#include <iostream>

//Initialsing our test machine
CPPFSM_INIT_STATE(TestMachine, State_1);

TEST_CASE("Start")
{
  CPPFSM_FORCE_STATE(TestMachine, State_1);
  TestMachine::start();
  
  SECTION("Checking event handling") {
    REQUIRE(TestMachine::assert_state<State_1>());
    TestMachine::event();
    REQUIRE(TestMachine::assert_state<State_2>());
  }
}
