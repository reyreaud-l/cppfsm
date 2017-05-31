#pragma once

#include <type_traits>

namespace cppfsm
{
  template <typename F, typename S>
  struct __state_bind
  {
    static S value;
    typedef S value_type;
  };
  
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
        state_current = get_state_ptr<S>();
        state_current->entry();
      }

      template <typename S>
      static constexpr state_ptr get_state_ptr(void)
      {
        return &__state_bind<F, S>::value;
      }

      static void event(void)
      {
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
