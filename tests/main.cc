#include "../src/fsm.hh"
#include <iostream>

int main()
{
  auto my_machine = fsm::Fsm("yolo"); 
  std::cout << my_machine.name_get() << std::endl;
}
