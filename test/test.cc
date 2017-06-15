#define CATCH_CONFIG_MAIN //Catch provide a main
#define __TEST_CPPFSM 
#include "catch.hpp"
#include "test.hh"
#include <iostream>

//Initialsing our test machine
CPPFSM_INIT_STATE(TestMachine, Init);

TEST_CASE("Start")
{
  CPPFSM_FORCE_STATE(TestMachine, Init);
  TestMachine::start();
  REQUIRE(TestMachine::assert_state<Init>());
  TestMachine::event();
  REQUIRE(TestMachine::assert_state<Middle>());
}

TEST_CASE("Transitions & Check")
{
  CPPFSM_FORCE_STATE(TestMachine, Init);
  TestMachine::start();

  REQUIRE(TestMachine::assert_state<Init>());
  TestMachine::event();
  REQUIRE(TestMachine::assert_state<Middle>());
  TestMachine::event();
  REQUIRE(TestMachine::assert_state<DeadEnd>());
  TestMachine::event();
  REQUIRE(TestMachine::assert_state<DeadEnd>());
}
