#pragma once

#include <type_traits>
#include <stdexcept>

namespace cppfsm
{
  //Represent a state value and it's type
  template <typename F, typename S>
  struct state_bind
  {
    static S value;
    typedef S value_type;
  };

  enum strictness
  {
    strict = 0,
    nonstrict
  };
  //Type is it's own value
  template<typename F, typename S>
  typename state_bind<F, S>::value_type state_bind<F, S>::value;

  template <typename F>
  class Cppfsm
  {
    using state_ptr = F *;

    public:
    static void start(void)
    {
      state_current->entry();
    }

    template <typename S>
    static void transit(void)
    {
      state_current->exit();
      //Update current state and then call entry
      state_current = get_state_ptr<S>();
      state_current->entry();
    }

    template <typename S, typename func>
    static typename std::enable_if<
      std::is_convertible<func, bool(*)()>::value, void>::type
    transit(func callee)
    {
      //Call checker function and proceed if true
      if (callee())
      {
        transit<S>();
      }
      else if (strict == strictness::strict)
      {
        //May need to change logic_error for something more explicit
        throw new std::logic_error("Invalid transition and strict enabled");
      }
    }

    template <typename S, typename func>
    static typename std::enable_if<
      std::is_convertible<func, void(*)()>::value, void>::type
    transit(func callee)
    {
      //Call checker function and proceed if true
      callee();
      transit<S>();
    }

    template <typename S>
    static constexpr state_ptr get_state_ptr(void)
    {
      //Return pointer on templated state
      return &state_bind<F, S>::value;
    }

    static void event(void)
    {
      //Dispatch event to current state
      state_current->react();
    }

    template <typename E>
    static void event(const E& payload)
    {
      //Dispatch event to current state with payload
      state_current->react(payload);
    }

    static state_ptr state_current;
    static strictness strict;

#ifdef __TEST_CPPFSM
    template <typename S>
    static bool bool_state(void)
    {
      return typeid(*state_current) == typeid(S);
    }
#define CPPFSM_FORCE_STATE(_MACHINE, _STATE) \
  cppfsm::Cppfsm<_MACHINE>::state_current = cppfsm::Cppfsm<_MACHINE>::get_state_ptr<_STATE>()
#endif
  };

  //Initial state initialisation
#define CPPFSM_INIT_STATE(_MACHINE, _STATE) \
  template <> \
  cppfsm::Cppfsm<_MACHINE>::state_ptr cppfsm::Cppfsm<_MACHINE>::state_current = \
  cppfsm::Cppfsm<_MACHINE>::get_state_ptr<_STATE>() 

  //Initial strictness initialisation
#define CPPFSM_INIT_STRICTNESS(_MACHINE, _STRICTNESS) \
  template <> \
  cppfsm::strictness cppfsm::Cppfsm<_MACHINE>::strict = \
  cppfsm::strictness::_STRICTNESS
  
  //Combination of both top define
#define CPPFSM_INIT(_MACHINE, _STATE, _STRICTNESS) \
  template <> \
  cppfsm::Cppfsm<_MACHINE>::state_ptr cppfsm::Cppfsm<_MACHINE>::state_current = \
  cppfsm::Cppfsm<_MACHINE>::get_state_ptr<_STATE>(); \
  template <> \
  cppfsm::strictness cppfsm::Cppfsm<_MACHINE>::strict = \
  cppfsm::strictness::_STRICTNESS

} //namespace cppfsm
