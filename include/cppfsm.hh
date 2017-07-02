#pragma once

#include <type_traits>
#include <string>
#include <memory>
#include <stdexcept>

namespace cppfsm
{
  /* Message and TransitMessage are classes for communication between
   * machines and listeners. 
   *
   * They were required because we need to send multiple informations
   * and C++ does not support virtual templated methods. Therefore listeners
   * could not be templated on a Machine/State type and extract infos
   * themselves.
   * */
  class Message
  {
    public:
    Message(std::string name)
    : state_name_(name)
    {}

    std::string state_name_get() { return state_name_; }
    std::string state_name_get() const { return state_name_; }

    private:
    std::string state_name_;
  };

  /* Represent a message for a transition.
   * */
  class TransitMessage
  {
    public:
    TransitMessage(std::string src, std::string dst)
      : src_(src)
      , dst_(dst)
    {}

    std::string src_get() { return src_; }
    std::string src_get() const { return src_; }
    std::string dst_get() { return dst_; }
    std::string dst_get() const { return dst_; }

    private:
    std::string src_;
    std::string dst_;
  };

  /* Represent a message for a transition with a check function.
   * */
  class CheckMessage : public TransitMessage
  {
    public:
    CheckMessage(std::string src, std::string dst, bool result)
    : TransitMessage(src, dst)
    , result_(result)
    {}

    bool result_get() { return result_; }
    bool result_get() const { return result_; }

    private:
    bool result_;
  };

  /* Actual listener. All functions are declared virtual pure and *need* to be
   * overriden by the user inheriting the class to register listeners into
   * machines.
   * */
  class Listener
  {
    public:
    virtual ~Listener() {};
    virtual void on_transit(const TransitMessage&) = 0;
    virtual void on_entry(const Message&) = 0;
    virtual void on_exit(const Message&) = 0;
    virtual void on_check(const CheckMessage&) = 0;
  };

  /* This part is actual CppFsm machine and logic. It heavily relies
   * on template to be as generic as possible. Refer to wiki/doc for
   * more in depth explanation on how to use the library.
   * */

  /* Represent a state and it's value
   * */
  template <typename F, typename S>
  struct state_bind
  {
    static S value;
    typedef S value_type;
  };

  /* Shorthand for state_bind value access
   * */
  template<typename F, typename S>
  typename state_bind<F, S>::value_type state_bind<F, S>::value;

  /* Represent the strictness of a machine. ie: if it will throw
   * or not on invalid transtion when check function return false
   * */
  enum strictness
  {
    nonstrict = 0,
    strict
  };

  template <typename F>
  class Fsm 
  {
    /* Alias on F for clearer understanding in code.
     * */
    using state_ptr = F *;

    public:

    /* Start function, should only be called once when starting the machine.
     * */
    static void start(void)
    {
      entry_state();
    }

    /* Transition function. Will exit current state, update internal ptr to the
     * new state and then entry the new state.
     * */
    template <typename S>
    static void transit(void)
    {
      exit_state();
      state_current_ = get_state_ptr<S>();
      dispatch_transit(TransitMessage(state_current_string_, typeid(S).name()));
      state_current_string_ = typeid(S).name();
      entry_state();
    }

    /* Transition function when a function is provided and it returns a boolean.
     * It will check for the return value of the said function and will proceed
     * with the transition or not.
     * */
    template <typename S, typename func>
    static typename std::enable_if<
      std::is_convertible<func, bool(*)()>::value, void>::type
    transit(func callee)
    {
      bool call_result = callee();
      dispatch_check(CheckMessage(state_current_string_,
            typeid(S).name(), call_result));
      if (call_result)
      {
        transit<S>();
      }
      else if (strict_ == strictness::strict)
      {
        //May need to change logic_error for something more explicit
        throw new std::logic_error("Invalid transition and strict enabled");
      }
    }

    /* Transition function when a function is provided and it returns anything
     * but a boolean. It will call the function and then proceed to transition.
     * */
    template <typename S, typename func>
    static typename std::enable_if<
      !std::is_convertible<func, bool(*)()>::value, void>::type
    transit(func callee)
    {
      callee();
      transit<S>();
    }

    /* Return a pointer on a templated state
     * */
    template <typename S>
    static constexpr state_ptr get_state_ptr(void)
    {
      //Return pointer on templated state
      return &state_bind<F, S>::value;
    }

    /* React to an event without any payload
     * */
    static void event(void)
    {
      state_current_->react();
    }

    /* React to an event with a payload (can be anything)
     * */
    template <typename P>
    static void event(const P& payload)
    {
      state_current_->react(payload);
    }

    /* Might need to switch to unique_ptr, implementation for listener
     * is a bit messy and should be reworker.
     * */
    static void register_listener(std::shared_ptr<Listener> first)
    {
      listeners_.emplace_back(first);
    }

    template <typename... Args>
    static void register_listener(std::shared_ptr<Listener> first, Args&&... args)
    {
      listeners_.emplace_back(first);
      register_listener(std::forward<Args>(args)...);
    }

    /* Clear all existing listeners in the machine
     * */
    static void clear_listeners()
    {
      listeners_.clear();
    }

    /* These attributes need to be public because of static as they
     * need to be initialized and are initialized outside of function.
     * */

    /* Represent current state
     * */
    static state_ptr state_current_;

    /* Represent current state name (needed because it is then hidden)
     * behind a pointer on Fsm */
    static std::string state_current_string_;

    /* Represent strictness of the machine
     * */
    static strictness strict_;

    /* Represent all listeners currently registered in the machine 
     * */
    static std::vector<std::shared_ptr<Listener>> listeners_;

    private:
    
    /* Call entry on current state and dispatch message
     * */
    static void entry_state(void)
    {
      state_current_->entry();
      dispatch_entry(Message(state_current_string_));
    }

    /* Call exit on current state and dispatch message
     * */
    static void exit_state(void)
    {
      state_current_->exit();
      dispatch_exit(Message(state_current_string_));
    }

    /* This part could be reworked to automatically dispatch
     * on the right function in the listeners.
     *
     * Should remove copy paste of loop and find a way to resolve
     * the right function to call.
     * */
    static void dispatch_entry(const Message& msg)
    {
      for (const auto& listener : listeners_)
        listener->on_entry(msg);
    }
    static void dispatch_exit(const Message& msg)
    {
      for (const auto& listener : listeners_)
        listener->on_exit(msg);
    }
    static void dispatch_transit(const TransitMessage& msg)
    {
      for (const auto& listener : listeners_)
        listener->on_transit(msg);
    }
    static void dispatch_check(const CheckMessage& msg)
    {
      for (const auto& listener : listeners_)
        listener->on_check(msg);
    }

    /* This function and macro are used only for and therefore are only
     * enabled when CPPFSM_TEST_ENABLE is enabled. You can use them if
     * you know what you are doing.
     *
     * Note that using a similar macro to force a state in a machine is
     * highly un-advised and you might need one only if your design is
     * poorly made. Try to improve your design and not force a machine
     * into a state.
     * */
#ifdef CPPFSM_TEST_ENABLE 
    public:
    template <typename S>
    static bool bool_state(void)
    {
      return state_current_string_ == typeid(S).name();
    }
#define CPPFSM_FORCE_STATE(_MACHINE, _STATE) \
  cppfsm::Fsm<_MACHINE>::state_current_ = \
    cppfsm::Fsm<_MACHINE>::get_state_ptr<_STATE>(); \
  cppfsm::Fsm<_MACHINE>::state_current_string_ = typeid(_STATE).name()
#endif
  };

  /* These macro are helpers to initiate machine in a state with a given
   * strictness parameter
   * */
#define CPPFSM_SET_STATE(_MACHINE, _STATE) \
  template <> \
  cppfsm::Fsm<_MACHINE>::state_ptr cppfsm::Fsm<_MACHINE>::state_current_ = \
  cppfsm::Fsm<_MACHINE>::get_state_ptr<_STATE>(); \
  template <> \
  std::string cppfsm::Fsm<_MACHINE>::state_current_string_ = \
  typeid(_STATE).name()

#define CPPFSM_SET_LISTENERS(_MACHINE) \
  template <> \
  std::vector<std::shared_ptr<cppfsm::Listener>> cppfsm::Fsm<_MACHINE>::listeners_\
  = std::vector<std::shared_ptr<cppfsm::Listener>>() \

#define CPPFSM_INIT_STATE(_MACHINE, _STATE) \
  CPPFSM_SET_LISTENERS(_MACHINE); \
  CPPFSM_SET_STATE(_MACHINE, _STATE) \

#define CPPFSM_INIT(_MACHINE, _STATE, _STRICTNESS) \
  CPPFSM_SET_LISTENERS(_MACHINE); \
  CPPFSM_SET_STATE(_MACHINE, _STATE); \
  template <> \
  cppfsm::strictness cppfsm::Fsm<_MACHINE>::strict_ = \
  cppfsm::strictness::_STRICTNESS \

} //namespace cppfsm
