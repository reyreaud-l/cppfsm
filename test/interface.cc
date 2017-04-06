#include "interface.hh"

namespace fsm
{
  void interface::start()
  {
    current_machine_.start();
  }

  void interface::transit(const Transition& transit)
  {
    current_machine_.transit(transit);
  }
  
  void interface::transit(const State& transit)
  {
    current_machine_.transit(transit);
  }
  
  void interface::transit(int transit)
  {
    current_machine_.transit(transit);
  }

  void interface::add_states(State& state)
  {
    current_machine_.add_states(state);
  }
  
  void interface::add_transitions(Transition& transition)
  {
    current_machine_.add_transitions(transition);
  }
  
  void interface::set_start_state(State& state)
  {
    current_machine_.set_start_state(state);
  }
      
  void interface::set_machine(std::string name)
  {
    current_machine_ = Fsm(name);
  }

  std::string interface::name_get()
  {
    return current_machine_.name_get();
  }
  
  int interface::id_get()
  {
    return current_machine_.id_get();
  }
}
