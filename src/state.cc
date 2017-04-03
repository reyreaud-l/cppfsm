#include "fsm.hh"

namespace fsm
{
  State::State(Fun state_func, std::string name)
  : id_(fresh_id())
  , state_func_(state_func)
  , name_(name)
  {}


  void State::entry()
  {
    state_func_();
  }

  void State::exit()
  {
    exit_func_();
  }

  void State::exit_func_set(Fun f)
  {
    exit_func_ = f;
  }

  bool State::operator==(State& other)
  {
    return id_ == other.id_;
  }

  bool State::operator==(const State& other) const
  {
    return id_ == other.id_;
  }

  bool State::operator!=(State& other)
  {
    return id_ != other.id_;
  }

  bool State::operator!=(const State& other) const
  {
    return id_ != other.id_;
  }

  std::string State::name_get()
  {
    return name_;
  }

  int State::id_get()
  {
    return id_;
  }
}
