#pragma once

#include <type_traits>

namespace cppfsm
{
  //Represent a state value and it's type
  template <typename F, typename S>
    struct __state_bind
    {
      static S value;
      typedef S value_type;
    };

  //Type is it's own value
  template<typename F, typename S>
    typename __state_bind<F, S>::value_type __state_bind<F, S>::value;

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
            "CPPFSM: Transit arg 1 is not a function");
        static_assert(std::is_same<typename std::result_of<check_func()>::type,
            bool>::value,
            "CPPFSM: Transit function check does not return bool");
        
        //Call checker function and proceed if true
        if (checker())
        {
          state_current->exit();
          //Update current state and then call entry
          state_current = get_state_ptr<S>();
          state_current->entry();
        }
      }
        
      template <typename S>
        static constexpr state_ptr get_state_ptr(void)
        {
          //Return pointer on templated state
          return &__state_bind<F, S>::value;
        }

      static void event(void)
      {
        //Dispatch event to current state
        state_current->react();
      }

      static state_ptr state_current;

#ifdef __TEST_CPPFSM
      template <typename S>
        static bool assert_state(void)
        {
          return typeid(*state_current) == typeid(S);
        }
#endif
    };

  //Initial state initialisation
#define CPPFSM_INIT_STATE(_MACHINE, _STATE) \
  template <> \
  cppfsm::Cppfsm<_MACHINE>::state_ptr cppfsm::Cppfsm<_MACHINE>::state_current = \
  cppfsm::Cppfsm<_MACHINE>::get_state_ptr<_STATE>() 
#ifdef __TEST_CPPFSM
#define CPPFSM_FORCE_STATE(_MACHINE, _STATE) \
  cppfsm::Cppfsm<_MACHINE>::state_current = cppfsm::Cppfsm<_MACHINE>::get_state_ptr<_STATE>()
#endif
}
