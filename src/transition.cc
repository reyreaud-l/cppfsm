#include "fsm.hh"

namespace fsm
{
  Transition::Transition(State from, State to, std::string name)
  : id_(fresh_id())
  , from_(from)
  , to_(to)
  , name_(name)
  {}
}
