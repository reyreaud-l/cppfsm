#include "fsm.hh"

namespace fsm
{
  Fsm::Fsm(std::string name)
  : id_(fresh_id())
  , name_(name)
  {}

  void Fsm::start()
  {
    if (current_state_)
      current_state_->entry();
    else
    {
      error("No current state");
    }
  }

  void Fsm::transit(const Transition& transit)
  {
    auto res = std::find_if(
        transitions_.begin(), transitions_.end(),
        [transit](const std::shared_ptr<Transition>& i)
        {
          return *i.get() == transit;
        });
    if (res == transitions_.end())
      error("Transition not found");
    else
      transit_check(**res);
  }

  void Fsm::transit(const State& state)
  {
    auto res = std::find_if(
        transitions_.begin(), transitions_.end(),
        [state, this](const std::shared_ptr<Transition>& i)
        {
          return *(i.get()->from_get())
            == *(this->current_state_)
            && *(i.get()->to_get()) == state;
        });
    if (res == transitions_.end())
      error("Transition not found");
    else
      transit_check(**res);
  }

  void Fsm::transit(int transit)
  {
    auto res = std::find_if(
        transitions_.begin(), transitions_.end(),
        [transit](const std::shared_ptr<Transition>& i)
        {
          return i.get()->id_get() == transit;
        });
    if (res == transitions_.end())
      error("Transition not found");
    else
      transit_check(**res);
  }

  void Fsm::transit_check(Transition& transit)
  {
    if (*transit.from_get() != *current_state_)
    {
      error("Invalid start state for transition");
      return;
    }
    current_state_->exit();
    current_state_ = transit.to_get();
    current_state_->entry();
  }

  void Fsm::error(std::string err)
  {
    std::cerr << err << " in machine " << id_;
    if (name_ != std::string(""))
      std::cerr << " named " << name_;
    std::cerr << std::endl;
  }

  void Fsm::set_start_state(State& start)
  {
    if (current_state_)
      return;
    current_state_ = std::make_shared<State>(start);
    add_states(start);
  }

  void Fsm::add_states(State& new_state)
  {
    states_.insert(std::make_shared<State>(new_state));
  }

  void Fsm::add_transitions(Transition& new_transition)
  {
    transitions_.insert(
        std::make_shared<Transition>(new_transition));
  }

  std::string Fsm::name_get()
  {
    return name_;
  }
}
