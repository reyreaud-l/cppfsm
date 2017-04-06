#include "fsm.hh"

namespace fsm
{
  int fresh_id()
  {
    static unsigned counter_ = 0;
    counter_++;
    return counter_;
  }

  void default_fun()
  {}
}
