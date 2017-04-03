#pragma once

#include <iostream>
#include <string>


namespace fsm
{
  int fresh_id();

  class State
  {
    public:
      State(std::string = "");

    protected:
      int id_;
      std::string name_;
  };

  class Transition
  {
    public:
      Transition(State, State, std::string = "");

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
      
      std::string name_get();

    protected:
      int id_;
      std::string name_;
  };

}
