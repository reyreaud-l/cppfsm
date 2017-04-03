#pragma once

#include <iostream>
#include <set>
#include <memory>
#include <algorithm>
#include <string>


namespace fsm
{
  //Unique ids for all elements
  int fresh_id();
  //Default exit function for a State
  void exit();
  //Declare type for state function
  typedef void (*Fun)();

  class State
  {
    public:
      State(Fun, std::string = "");

      void entry();
      void exit();

      bool operator==(State&);
      bool operator==(const State&) const;

      void exit_func_set(Fun);

      std::string name_get();
      int id_get();
    protected:
      int id_;
      Fun state_func_;
      Fun exit_func_ = fsm::exit;
      std::string name_;
  };

  class Transition
  {
    public:
      Transition(State, State, std::string = "");

      bool operator==(Transition&);
      bool operator==(const Transition&) const;

      std::string name_get();
      int id_get();
    protected:
      int id_;
      State from_;
      State to_;
      std::string name_;
  };

  class Fsm
  {
    public:
      Fsm(std::string name = "");
      
      void start();
      void transit(const Transition&);
      void transit(int);

      void add_states(State&);
      void add_transitions(Transition&);
      void set_start_state(State&);
      
      std::string name_get();
      int id_get();

    protected:
      void error(std::string);
      int id_;
      std::string name_;
      std::shared_ptr<State> current_state_ = nullptr;
      std::set<std::shared_ptr<State>> states_;
      std::set<std::shared_ptr<Transition>> transitions_;
  };
}
