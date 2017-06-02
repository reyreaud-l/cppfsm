#include <cppfsm.hh>

class TestMachine : public cppfsm::Cppfsm<TestMachine>
{
  public:
  void virtual entry(void) { };
  void virtual exit(void) { };
  void virtual react(void) { };
  bool virtual check(void) { return true; };
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

  bool check(void)
  {
    std::cout << "Check called\n";
    return true;
  }
};

class State_2 : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}

  bool check(void)
  {
    std::cout << "Check 2 called\n";
    return true;
  }
};

