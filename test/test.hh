#include <cppfsm.hh>

class TestMachine : public cppfsm::Fsm<TestMachine>
{
  public:
  void virtual entry(void) { };
  void virtual exit(void) { };
  void virtual react(void) { };
  void virtual react(int) { };
  bool virtual check(void) { return true; };
};


//TMP
class Init;
class Middle;
class DeadEnd;
class Payload;

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

class DeadEnd : public TestMachine
{
  public:
  void entry(void) override
  {}

  void exit(void) override
  {}

  bool check(void) override
  {
    return false;
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

class CallFunc : public TestMachine
{
  public:
    void react(void) override
    {
      auto checker = []()
      {
        return 10;
      };
      transit<DeadEnd>(checker);
    }
};

class Payload : public TestMachine
{
  public:
    void react(int payload) override
    {
      (void)payload; //avoid warning unused variable
      transit<DeadEnd>();
    }
};
