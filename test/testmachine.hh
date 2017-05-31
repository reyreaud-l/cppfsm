#pragma once

#include <cppfsm.hh>
#include <iostream>

class TestMachine : public cppfsm::Cppfsm<TestMachine>
{
  public:
  void virtual entry(void) { };
  void virtual exit(void) { };
  void virtual react(void) { };
};


//TMP
class State_2;

class State_1 : public TestMachine
{
  public:
  void entry(void) override
  {
    std::cout << "State_1 started\n";
  }

  void exit(void) override
  {
    std::cout << "State_1 exited\n";
  }

  void react(void) override
  {
    std::cout << "State_1 received event, transitioning to state 2\n";
    transit<State_2>();
  }
};

class State_2 : public TestMachine
{
  public:
  void entry(void) override
  {
    std::cout << "State_2 started\n";
  }

  void exit(void) override
  {
    std::cout << "State_2 exited\n";
  }
};
