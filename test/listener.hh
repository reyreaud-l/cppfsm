#pragma once
#include "cppfsm.hh"
#include <iostream>

//You should not do this using;
using namespace cppfsm;

class MyListener : public Listener
{
  public:
    void on_transit(const TransitMessage& m) override
    {
      std::cout << "Transit: " << m.src_get() << " -> " << m.dst_get() << "\n";
    }

    void on_entry(const Message& m) override
    {
      std::cout << "Entry: " << m.state_name_get() << "\n";
    }

    void on_exit(const Message& m) override
    {
      std::cout << "Exit: " << m.state_name_get() << "\n";
    }

    void on_check(const CheckMessage& m) override
    {
      std::cout << "Check: " << m.src_get() << " -> " << m.dst_get()
        << " result is " << m.result_get() << "\n";
    }
};
