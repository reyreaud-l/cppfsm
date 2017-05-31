#include <cppfsm.hh>
#include "testmachine.hh"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

CPPFSM_INIT_STATE(TestMachine, State_1);

int main(void)
{
  TestMachine::start();
  TestMachine::event();
}
