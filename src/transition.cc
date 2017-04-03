#include "fsm.hh"

namespace fsm
{
  Transition::Transition(State from, State to, std::string name)
  : id_(fresh_id())
  , from_(from)
  , to_(to)
  , name_(name)
  {}
  
  bool Transition::operator==(Transition& other)
  {
    return from_ == other.from_ && to_ == other.to_;
  }

  bool Transition::operator==(const Transition& other) const
  {
    return from_ == other.from_ && to_ == other.to_;
  }

  std::string Transition::name_get()
  {
    return name_;
  }

  int Transition::id_get()
  {
    return id_;
  }
}
