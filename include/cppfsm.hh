#pragma once

#include <type_traits>

namespace cppfsm
{
  /*Helper structure to check if a class implement a given method
  template<typename, typename T>
  struct has_check
  {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
  };

  //Template specialization to check if class implem check
  //FOR NOW USELESS, BECAUSE CANNOT FUNCTION WILL ALWAYS BE PRESENT
  //AS IT IS NOT POSSIBLE TO DECLTYPE ON DYNAMIC TYPE, THEREFORE
  //IT IS NOT POSSIBLE TO CHECK FOR METHOD PRECENSE AT RUNTIME
  //ON DERIVED CLASS. FOR NOW CHECK IS IMPLEMENTED AS VIRTUAL IN
  //BASE CLASS.
  template<typename C, typename Ret, typename... Args>
  struct has_check<C, Ret(Args...)>
  {
    private:
      template<typename T>
      static constexpr auto check(T*)
      -> typename std::is_same<
        decltype(std::declval<T>().check(std::declval<Args>()...)),
        Ret>::type;

      template<typename T>
      static constexpr std::false_type check(...);

      typedef decltype(check<C>(0)) type;

      static bool eval_inner(C* c, std::true_type)
      {
        std::cout << "ok\n";
        return c->check();
      }

      static bool eval_inner(C*, std::false_type)
      {
        std::cout << "no-op\n";
        return false;
      }

    public:
      static bool eval(C* c)
      {
        return eval_inner(c, type());
      }

      static const bool value = type::value;
  }; */

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
        //Check for check method presence and abort if present and
        //abort if it return false
        /* if (has_check<F, bool(void)>::value)
        {
          std::cout << "Check detected\n";
          if (!has_check<F, bool(void)>::eval(state_current))
            return;
        } */
        if (!state_current->check())
          return;
        state_current->exit();
        //Update current state and then call entry
        state_current = get_state_ptr<S>();
        state_current->entry();
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
