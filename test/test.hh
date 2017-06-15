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
class Init;
class Middle;
class DeadEnd;

class Init : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}

  void react(void) override
  {
    transit<Middle>();
  }
};

class Middle : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}

  void react(void) override
  {
    transit<DeadEnd>();
  }
};

class CheckTrue : public TestMachine
{
  public:
    void react(void) override
    {
      auto checker = []()
      {
        return true;
      };
      transit<DeadEnd>(checker);
    }
};

class CheckFalse : public TestMachine
{
  public:
    void react(void) override
    {
      auto checker = []()
      {
        return false;
      };
      transit<DeadEnd>(checker);
    }
};

class DeadEnd : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}

  bool check(void)
  {
    return false;
  }
};
