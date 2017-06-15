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

    template <typename S, typename check_func>
    static void transit(check_func checker)
    {
      //Check for function and return type is bool
      static_assert(!std::is_function<check_func>::value,
          "Transit arg 1 is not a function");
      static_assert(std::is_same<typename std::result_of<check_func()>::type,
          bool>::value,
          "Transit arg 1 does not return bool");
      
      //Call checker function and proceed if true
      if (checker())
      {
        state_current->exit();
        //Update current state and then call entry
        state_current = get_state_ptr<S>();
        state_current->entry();
      }
      //if strict is enabled, throw and report
      else if (strict == strictness::strict)
      {
        //May need to change logic_error for something more explicit
        throw new std::logic_error("Invalid transition and strict enabled");
      }
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
