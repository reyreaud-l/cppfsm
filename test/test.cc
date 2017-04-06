#include "fsm.hh"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mockinterface.hh"
#include <iostream>

TEST(get_set, id_get)
{
  fsm::MockInterface interface;
  EXPECT_CALL(interface, set_machine("motor"))
    .Times(1);
  EXPECT_CALL(interface, id_get())
    .Times(1);

  interface.set_machine("motor");
  EXPECT_EQ(interface.id_get(), 0);
}

void statefunction1()
{
}

void statefunction1_exit()
{
}

void statefunction2()
{
}

int main(int argc, char** argv)
{
  auto my_machine = fsm::Fsm("Motor"); 
  auto state_1 = fsm::State(statefunction1, "iamstate1");
  auto state_2 = fsm::State(statefunction2, "iamstate2");
  auto transit_1 = fsm::Transition(state_1, state_2, "transit1");
  auto transit_2 = fsm::Transition(state_2, state_1, "transit2");
  std::cout << my_machine.id_get() << std::endl;
  my_machine.add_states(state_1);
  my_machine.add_states(state_2);
  my_machine.add_transitions(transit_1);
  my_machine.add_transitions(transit_2);
  state_1.exit_func_set(statefunction1_exit);
  my_machine.set_start_state(state_1);
  my_machine.start();
  my_machine.transit(transit_1);
  my_machine.transit(transit_2);
  my_machine.transit(state_2);

  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
