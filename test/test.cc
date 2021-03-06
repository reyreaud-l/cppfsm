#define CATCH_CONFIG_MAIN //Catch provide a main
#define CPPFSM_TEST_ENABLE
#include "catch.hpp"
#include "test.hh"
#include "listener.hh"
#include "printer.hh"
#include <iostream>

//Initialsing our test machine
CPPFSM_INIT(TestMachine, Init, nonstrict);

TEST_CASE("Start")
{
  CPPFSM_FORCE_STATE(TestMachine, Init);
  TestMachine::start();
  REQUIRE(TestMachine::bool_state<Init>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<Middle>());
}

TEST_CASE("Transitions")
{
  CPPFSM_FORCE_STATE(TestMachine, Init);
  TestMachine::start();

  REQUIRE(TestMachine::bool_state<Init>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<Middle>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());
}

TEST_CASE("Transitions true")
{
  CPPFSM_FORCE_STATE(TestMachine, CheckTrue);
  TestMachine::start();
  
  REQUIRE(TestMachine::bool_state<CheckTrue>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());
}

TEST_CASE("Transitions false")
{
  CPPFSM_FORCE_STATE(TestMachine, CheckFalse);
  TestMachine::start();
  
  REQUIRE(TestMachine::bool_state<CheckFalse>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<CheckFalse>());
}

TEST_CASE("Transitions call func")
{
  CPPFSM_FORCE_STATE(TestMachine, CallFunc);
  TestMachine::start();
  
  REQUIRE(TestMachine::bool_state<CallFunc>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());
}

TEST_CASE("Event payload")
{
  CPPFSM_FORCE_STATE(TestMachine, Payload);
  TestMachine::start();
  
  REQUIRE(TestMachine::bool_state<Payload>());
  TestMachine::event(42);
  REQUIRE(TestMachine::bool_state<DeadEnd>());
}

TEST_CASE("Listener")
{
  CPPFSM_FORCE_STATE(TestMachine, Init);
  auto listener = std::make_shared<MyListener>();
  TestMachine::register_listener(listener);
  TestMachine::start();

  REQUIRE(TestMachine::bool_state<Init>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<Middle>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());

  TestMachine::clear_listeners();
}

TEST_CASE("Flow graph Listener")
{
  CPPFSM_FORCE_STATE(TestMachine, Init);
  auto listener = std::make_shared<FlowPrinter>("flowgraph.gv");
  TestMachine::register_listener(listener);
  TestMachine::start();

  REQUIRE(TestMachine::bool_state<Init>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<Middle>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());
  TestMachine::event();
  REQUIRE(TestMachine::bool_state<DeadEnd>());

  TestMachine::clear_listeners();
}
