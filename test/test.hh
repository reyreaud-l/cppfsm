#include <cppfsm.hh>

class TestMachine : public cppfsm::Cppfsm<TestMachine>
{
  public:
  void virtual entry(void) { };
  void virtual exit(void) { };
  void virtual react(void) { };
  void virtual check(void) { };
};


//TMP
class State_2;

class State_1 : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}

  void react(void) override
  {
    transit<State_2>();
  }
};

class State_2 : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}
};

