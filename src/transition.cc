#include "fsm.hh"

namespace fsm
{
  Transition::Transition(State from, State to,
      std::string name)
  : id_(fresh_id())
  , from_(std::make_shared<State>(from))
  , to_(std::make_shared<State>(to))
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

  std::shared_ptr<State> Transition::from_get()
  {
    return from_;
  }

  const std::shared_ptr<State> Transition::from_get() const
  {
    return from_;
  }

  std::shared_ptr<State> Transition::to_get()
  {
    return to_;
  }

  const std::shared_ptr<State> Transition::to_get() const
  {
    return to_;
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
