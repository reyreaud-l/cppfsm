#pragma once

#include <string>
#include "fsm.hh"

namespace fsm
{
  class interface
  {
    public:
      interface() {}
      virtual ~interface() {}
      
      virtual void start();
      virtual void transit(const Transition&);
      virtual void transit(const State&);
      virtual void transit(int);

      virtual void add_states(State&);
      virtual void add_transitions(Transition&);
      virtual void set_start_state(State&);
      
      virtual void set_machine(std::string = "");

      virtual std::string name_get();
      virtual int id_get();

    protected:
      Fsm current_machine_;
  };
}
