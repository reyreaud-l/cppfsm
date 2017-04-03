#include "fsm.hh"

namespace fsm
{
  Fsm::Fsm(std::string name)
  : id_(fresh_id())
  , name_(name)
  {}

  std::string Fsm::name_get()
  {
    return name_;
  }

  int fresh_id()
  {
    static unsigned counter_ = 0;
    counter_++;
    return counter_;
  }
}
