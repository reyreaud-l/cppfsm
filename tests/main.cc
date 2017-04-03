#include "fsm.hh"
#include <iostream>

void statefunction1()
{
  std::cout << "State 1 is running\n";
}

void statefunction1_exit()
{
  std::cout << "State 1 is exiting\n";
}

void statefunction2()
{
  std::cout << "State 2 is running\n";
}

int main()
{
  auto my_machine = fsm::Fsm("yolo"); 
  auto state_1 = fsm::State(statefunction1, "iamstate1");
  auto state_2 = fsm::State(statefunction2, "iamstate2");
  auto transit_1 = fsm::Transition(state_1, state_2, "transit1");
  std::cout << my_machine.name_get() << std::endl;
  my_machine.add_states(state_1);
  my_machine.add_states(state_2);
  my_machine.add_transitions(transit_1);
  state_1.exit_func_set(statefunction1_exit);
  my_machine.set_start_state(state_1);
  my_machine.start();
  my_machine.transit(transit_1);
}
